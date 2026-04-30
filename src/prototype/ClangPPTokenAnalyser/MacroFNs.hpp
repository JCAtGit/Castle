#pragma once
#include "clang/AST/Decl.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include "llvm/ADT/ArrayRef.h"
#include <vector>

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"

using namespace clang;

struct MacroExpansionInfo {
    std::vector<clang::syntax::TokenBuffer::Expansion> before;
    std::vector<clang::syntax::TokenBuffer::Expansion> after;
    std::vector<clang::syntax::TokenBuffer::Expansion> containing;
	std::vector<clang::syntax::TokenBuffer::Expansion> inside;    	// macro is inside the statement
};

// Detects ALL macros before, after and encapsulating.
MacroExpansionInfo detectMacrosAroundStatement(
    const clang::Stmt *S,
    const clang::syntax::TokenBuffer &Tokens,
    const clang::SourceManager &SM,
    const clang::LangOptions &LangOpts) {

	MacroExpansionInfo result;
    if (!S) return result;

    clang::SourceRange stmtRange = S->getSourceRange();
    if (stmtRange.isInvalid()) return result;

    // Get all spelled tokens for the main file
    clang::FileID mainFileID = SM.getMainFileID();
    llvm::ArrayRef<clang::syntax::Token> allSpelled = Tokens.spelledTokens(mainFileID);
    if (allSpelled.empty()) return result;

    // Get all macro expansions in the entire main file
    auto allExpansions = Tokens.expansionsOverlapping(allSpelled);

    for (const auto &exp : allExpansions) {
        if (exp.Spelled.empty()) continue;

        // Source range of the macro expansion (spelled tokens)
        clang::SourceRange expRange(exp.Spelled.front().location(),
                                    exp.Spelled.back().location());

        // Check if expansion contains the statement
        bool containsStmt = SM.isPointWithin(stmtRange.getBegin(), expRange.getBegin(), expRange.getEnd()) &&
                            SM.isPointWithin(stmtRange.getEnd(), expRange.getBegin(), expRange.getEnd());
        if (containsStmt) {
            result.containing.push_back(exp);
            continue;
        }

        // Check if expansion is entirely before the statement
        if (SM.isBeforeInTranslationUnit(expRange.getEnd(), stmtRange.getBegin())) {
            result.before.push_back(exp);
        }
        // Check if expansion is entirely after the statement
        else if (SM.isBeforeInTranslationUnit(stmtRange.getEnd(), expRange.getBegin())) {
            result.after.push_back(exp);
        }
        // Otherwise, it partially overlaps (rare) – you can ignore or categorize separately
    }

    return result;
}

// Detects only adjacent macros.
MacroExpansionInfo detectAdjacentMacros(
    const clang::Stmt *S,
    const clang::syntax::TokenBuffer &Tokens,
    const clang::SourceManager &SM,
    const clang::LangOptions &LangOpts) {

	MacroExpansionInfo result;
   if (!S) return result;

   clang::SourceRange stmtRange = S->getSourceRange();
   if (stmtRange.isInvalid()) return result;

   // Get all spelled tokens for the main file
   clang::FileID mainFileID = SM.getMainFileID();
   llvm::ArrayRef<clang::syntax::Token> allSpelled = Tokens.spelledTokens(mainFileID);
   if (allSpelled.empty()) return result;

   // Find token indices that belong to the statement
   size_t firstStmtIdx = allSpelled.size();
   size_t lastStmtIdx = 0;
   for (size_t i = 0; i < allSpelled.size(); ++i) {
       if (SM.isPointWithin(allSpelled[i].location(), stmtRange.getBegin(), stmtRange.getEnd())) {
           firstStmtIdx = std::min(firstStmtIdx, i);
           lastStmtIdx = i;
       }
   }
   if (firstStmtIdx > lastStmtIdx) return result;

   // Get all macro expansions in the main file
   auto allExpansions = Tokens.expansionsOverlapping(allSpelled);

   for (const auto &exp : allExpansions) {
       if (exp.Spelled.empty()) continue;
       // Skip non-identifier macro names (like '#')
       if (!tok::isAnyIdentifier(exp.Spelled.front().kind()))
           continue;

       // Find token indices for this expansion
       size_t expFirstIdx = allSpelled.size();
       size_t expLastIdx = 0;
       for (size_t i = 0; i < allSpelled.size(); ++i) {
           if (allSpelled[i].location() == exp.Spelled.front().location()) {
               expFirstIdx = i;
               for (size_t j = i; j < allSpelled.size(); ++j) {
                   if (allSpelled[j].location() == exp.Spelled.back().location()) {
                       expLastIdx = j;
                       break;
                   }
               }
               break;
           }
       }
       if (expFirstIdx > expLastIdx) continue;

       // --- Classification ---
       if (expFirstIdx <= firstStmtIdx && expLastIdx >= lastStmtIdx) {
           result.containing.push_back(exp);           // macro fully contains statement
       }
       else if (expFirstIdx >= firstStmtIdx && expLastIdx <= lastStmtIdx) {
           result.inside.push_back(exp);               // macro is inside statement
       }
       else if (expLastIdx + 1 == firstStmtIdx) {
           result.before.push_back(exp);               // directly before
       }
       else if (lastStmtIdx + 1 == expFirstIdx) {
           result.after.push_back(exp);                // directly after
       }
       // else overlapping but not adjacent – ignore
   }

    return result;
}

void printMacroExpansionInfo(const MacroExpansionInfo &Info,
                             const clang::syntax::TokenBuffer &Tokens,
                             const clang::SourceManager &SM) {
    auto printMacros = [&](const char *title,
                           const std::vector<clang::syntax::TokenBuffer::Expansion> &vec) {
        if (vec.empty()) return;
        llvm::outs() << title << ":\n";
        for (const auto &Exp : vec) {
            if (!Exp.Spelled.empty()) {
	            // Only print if the token is a proper identifier (not '#', '(', etc.)
				const clang::syntax::Token &FirstTok = Exp.Spelled.front();
	            if (!tok::isAnyIdentifier(FirstTok.kind())) {
					continue;
				}
                StringRef name = Exp.Spelled.front().text(SM);
                llvm::outs() << "  " << name;
                if (Exp.Expanded.empty())
                    llvm::outs() << " (empty expansion)";
                llvm::outs() << "\n";
            }
        }
    };

    printMacros("Macros before the statement", Info.before);
    printMacros("Macros after the statement", Info.after);
    printMacros("Macros containing the statement", Info.containing);
	printMacros("Macros inside the statement", Info.inside);
}

void MacroPrinter(Stmt *S, syntax::TokenBuffer &Tokens, SourceManager &SM){
	// Ignore statements that are not from the main file
	SourceLocation Begin = S->getBeginLoc();
   	if (!SM.isWrittenInMainFile(Begin) || Begin.isInvalid()) {
		return;
	}
   SourceRange Range = S->getSourceRange();
   const syntax::Token *SpelledTok = Tokens.spelledTokenContaining(Begin);
   
   llvm::outs() << "\n=== Statement " << S->getStmtClassName() << " at " << Range.getBegin().printToString(SM) << "\n";

   // --- Expanded tokens ---
   llvm::outs() << "Expanded tokens:\n";
   for (const syntax::Token &Tok : Tokens.expandedTokens(Range)) {
       llvm::outs() << "  '" << Tok.text(SM) << "' (kind=" << Tok.kind() << ")\n";
   }

   // --- Spelled tokens (original source) ---
   llvm::outs() << "Spelled tokens:\n";
   auto tokensSpelledExpanded = Tokens.spelledForExpanded(Tokens.expandedTokens(Range));
   if (tokensSpelledExpanded) {
       for (const syntax::Token &Tok : *tokensSpelledExpanded) {
           llvm::outs() << "  '" << Tok.text(SM) << "' (kind=" << Tok.kind() << ")\n";
       }
   } else {
       llvm::outs() << "  (no spelled tokens for this range)\n";
   }
   
	
   // --- Find macro expansions overlapping this statement ---
   // Get all spelled tokens for the main file
   FileID MainFileID = SM.getMainFileID();
   llvm::ArrayRef<syntax::Token> AllSpelled = Tokens.spelledTokens(MainFileID);

   // Find the first and last spelled token that lie inside the statement's source range
   const syntax::Token *FirstTok = nullptr, *LastTok = nullptr;
   for (const syntax::Token &Tok : AllSpelled) {
       if (SM.isPointWithin(Tok.location(), Range.getBegin(), Range.getEnd())) {
           if (!FirstTok) FirstTok = &Tok;
           LastTok = &Tok;
       }
   }

   if (FirstTok && LastTok) {
       // Build an ArrayRef of the contiguous spelled tokens for this statement
       llvm::ArrayRef<syntax::Token> SpelledRange(FirstTok, (LastTok - FirstTok) + 1);
       auto Overlapping = Tokens.expansionsOverlapping(SpelledRange);
       
       for (const auto &Exp : Overlapping) {
           // Only consider expansions whose first token is a proper identifier
           if (!Exp.Spelled.empty() && tok::isAnyIdentifier(Exp.Spelled.front().kind())) {
               StringRef MacroName = Exp.Spelled.front().text(SM);
               llvm::outs() << "Macro expansion detected: " << MacroName << "\n";
           }
       }
   }	
}

std::optional<std::string> getMacroNameIfStmtStartsWithMacro(
    const clang::Stmt *S,
    const clang::syntax::TokenBuffer &Tokens,
    const clang::SourceManager &SM,
    const clang::LangOptions &LangOpts) {

    if (!S) return std::nullopt;

    clang::SourceLocation Begin = S->getBeginLoc();
    if (Begin.isInvalid()) return std::nullopt;

    // 1. Convert any macro location to its source file expansion location.
    clang::SourceLocation ExpansionLoc = SM.getExpansionLoc(Begin);
    if (ExpansionLoc.isInvalid()) return std::nullopt;

    // 2. Use the *expansion location* to get the spelled token.
    const clang::syntax::Token *SpelledTok = Tokens.spelledTokenContaining(ExpansionLoc);
    if (!SpelledTok) {
        // Optional: Log a message for debugging if needed.
        // llvm::errs() << "Warning: Could not find spelled token at location.\n";
        return std::nullopt;
    }

    // 3. Check if this token is the start of a macro expansion.
    auto Exp = Tokens.expansionStartingAt(SpelledTok);
    if (!Exp) return std::nullopt;

    // 4. Ensure the expansion has spelled tokens and the first one is an identifier.
    if (Exp->Spelled.empty()) return std::nullopt;
    const clang::syntax::Token &FirstTok = Exp->Spelled.front();
    if (!tok::isAnyIdentifier(FirstTok.kind()))
        return std::nullopt;

    // 5. Return the macro name.
    return FirstTok.text(SM).str();
}
