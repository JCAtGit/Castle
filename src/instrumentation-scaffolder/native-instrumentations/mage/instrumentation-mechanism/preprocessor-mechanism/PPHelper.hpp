#pragma once

#include "clang/AST/Attr.h"
#include "clang/AST/Decl.h"
#include "clang/AST/DeclBase.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include "clang/Lex/Lexer.h"
#include "PPDatastructures.hpp"
#include "clang/Basic/LangOptions.h"

// --------------------------------------------------------------
// Helper: get token indices for a source range
// --------------------------------------------------------------
struct TokenRange {
    size_t firstIdx, lastIdx;
};
static std::optional<TokenRange> getTokenIndicesForRange(
    const SourceRange &SR, llvm::ArrayRef<syntax::Token> SpelledTokens,
    const SourceManager &SM) {
    size_t first = SpelledTokens.size(), last = 0;
    bool foundFirst = false;
    for (size_t i = 0; i < SpelledTokens.size(); ++i) {
        SourceLocation loc = SpelledTokens[i].location();
        if (loc == SR.getBegin()) {
            first = i;
            foundFirst = true;
        }
        if (loc == SR.getEnd()) {
            last = i;
            break;
        }
    }
    if (!foundFirst || first > last) return std::nullopt;
    return TokenRange{first, last};
}


// --------------------------------------------------------------
// Printing helpers
// --------------------------------------------------------------
void printEvent(const RecordedPPConstruct &ev, llvm::raw_ostream &OS) {
    if (ev.isMacro) {
        OS << "macro " << ev.name;
    } else {
        switch (ev.kind) {
            case DirectiveKind::Include: OS << "#include " << ev.name; break;
            case DirectiveKind::If:      OS << "#if"; break;
            case DirectiveKind::Ifdef:   OS << "#ifdef " << ev.name; break;
            case DirectiveKind::Ifndef:  OS << "#ifndef " << ev.name; break;
            case DirectiveKind::Elif:    OS << "#elif"; break;
            case DirectiveKind::Else:    OS << "#else"; break;
            case DirectiveKind::Endif:   OS << "#endif"; break;
            case DirectiveKind::Pragma:  OS << "#pragma"; break;
        }
    }
}

void printEventVectors(const PPEventVectors &vec, const SourceManager &SM) {
    auto printList = [&](const char *title, const std::vector<RecordedPPConstruct> &list) {
        if (list.empty()) return;
        llvm::outs() << title << ":\n";
        for (const auto &ev : list) {
            llvm::outs() << "  ";
            printEvent(ev, llvm::outs());
            llvm::outs() << " at " << ev.range.getBegin().printToString(SM) << "\n";
        }
    };
    printList("  Events containing the statement", vec.containing);
    printList("  Events inside the statement", vec.inside);
    printList("  Events before (adjacent)", vec.before);
    printList("  Events after (adjacent)", vec.after);
}

// --------------------------------------------------------------
// PP Predicates
// --------------------------------------------------------------
bool isPPSafeStmtMarking(PPEventVectors ppEV){
	return (
		ppEV.before.empty() && 
		ppEV.after.empty() && 
		ppEV.containing.empty()  
//		ppEV.inside.empty()
	);
}

bool isPPSafeDeclMarking(PPEventVectors ppEV){
	return (
		ppEV.before.empty() && 
		ppEV.after.empty() && 
		ppEV.containing.empty() 
//		ppEV.inside.empty()
	);	
}

void addOrCreateGNUOptnone(FunctionDecl *FD, Rewriter &Rewriter,
                              const SourceManager &SM,
                              const LangOptions &LangOpts) {
								llvm::errs() << "addOrCreateGNUOptnone called for " << FD->getName() << "\n";

    // Iterate over all attributes attached to the function.
    for (const auto *Attr : FD->attrs()) {
        // We are only interested in GNU attributes.
        if (Attr->getSyntax() != AttributeCommonInfo::AS_GNU) {
            continue;
        }

        SourceRange range = Attr->getRange();
        if (range.isInvalid()) {
            continue;
        }

        // Get the exact source code of the attribute as a string.
        CharSourceRange charRange = Lexer::getAsCharRange(range, SM, LangOpts);
        StringRef attrText = Lexer::getSourceText(charRange, SM, LangOpts);
        if (attrText.empty()) {
            continue;
        }

        std::string newAttrText;

        // Determine if the attribute list inside the parentheses is empty.
        // We look for the pattern "(()" for the start of the list and "))" for the end.
        if (attrText.find("()") != StringRef::npos) {
            // Case 1: The attribute list is empty: " __attribute__(()) "
            // We replace the whole attribute with a new one that contains 'optnone'.
            newAttrText = "__attribute__((optnone))";
        } else {
            // Case 2: The attribute list is not empty.
            // We need to insert 'optnone, ' inside the parentheses.
            // Find the start of the argument list (the second '(').
            size_t start = attrText.find('(');
            if (start == StringRef::npos) continue;
            start = attrText.find('(', start + 1);
            if (start == StringRef::npos) continue;

            // Build the new attribute text.
            newAttrText = attrText.str();
            newAttrText.insert(start + 1, "optnone, ");
        }

        // Replace the original attribute's source code with the new one.
        Rewriter.ReplaceText(range, newAttrText);
        return; // Assume we only modify the first GNU attribute found.
    }

    // No GNU attribute found at all: create one from scratch.
    Rewriter.InsertText(FD->getBeginLoc(), "__attribute__((optnone)) ");
}

