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

// --------------------------------------------------------------
// Attribute helpers
// --------------------------------------------------------------
#include "clang/AST/Decl.h"
#include "clang/AST/Attr.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

void addOrCreateGNUOptnone(FunctionDecl *FD, Rewriter &Rewriter,
                           const SourceManager &SM, const LangOptions &LangOpts) {
    // 1. Already has optnone? Nothing to do.
    if (FD->hasAttr<OptimizeNoneAttr>())
        return;

    // 2. Conflict detection using Clang attribute methods.
    //    Attributes that are incompatible with optnone.
    if (FD->hasAttr<AlwaysInlineAttr>() || FD->hasAttr<NakedAttr>() ||
        FD->hasAttr<WeakAttr>() || FD->hasAttr<WeakRefAttr>()) {
        return; // Do not add optnone if any conflict exists.
    }
	// Only rewrite if this is a function definition (has a body) AND it is at file scope.
	if (!FD->isThisDeclarationADefinition()) return;
	if (!FD->getLexicalDeclContext()->isFileContext()) return;

	// Additionally, ensure the source location is in a file with a recognized extension
	// (e.g., .c, .h) but NOT in a system header.
	if (SM.isInSystemHeader(FD->getLocation())) return;
	if (SM.isInExternCSystemHeader(FD->getLocation())) return;

	// Reject if the declaration is inside a struct/union (shouldn't happen for FunctionDecl, but double-check)
	if (FD->getDeclContext()->isRecord()) return;
	
    // Helper to check if a source location is safe for rewriting
    // (not inside a macro expansion and maps to a file location).
    auto isLocationSafe = [&](SourceLocation Loc) -> bool {
        if (Loc.isInvalid()) return false;
        if (Loc.isMacroID()) return false;
        CharSourceRange TokenRange = CharSourceRange::getTokenRange(Loc, Loc);
        CharSourceRange FileRange = Lexer::makeFileCharRange(TokenRange, SM, LangOpts);
        if (FileRange.isInvalid()) return false;
        return FileRange.getBegin().isFileID();
    };

    auto isRangeSafe = [&](SourceRange R) -> bool {
        return R.isValid() && isLocationSafe(R.getBegin()) && isLocationSafe(R.getEnd());
    };

    // 3. Try to merge into an existing GNU __attribute__((...)).
    //    Prefer an attribute whose source range is safe and that appears
    //    before the function body (if any). For functions without a body
    //    (declarations) we simply insert a new attribute.
    if (FD->isThisDeclarationADefinition()) {
        // Find the opening brace location of the function body.
        Stmt *Body = FD->getBody();
        if (Body) {
            SourceLocation BraceLoc = Body->getBeginLoc();
            // Find an existing GNU attribute that ends before the brace.
            Attr *TargetAttr = nullptr;
            for (auto *A : FD->attrs()) {
                if (A->getSyntax() != AttributeCommonInfo::AS_GNU)
                    continue;
                SourceRange R = A->getRange();
                if (!isRangeSafe(R))
                    continue;
                if (SM.isBeforeInTranslationUnit(R.getEnd(), BraceLoc)) {
                    TargetAttr = A;
                    break; // take the first such attribute
                }
            }

            if (TargetAttr) {
                SourceRange range = TargetAttr->getRange();
                CharSourceRange charRange = Lexer::getAsCharRange(range, SM, LangOpts);
                StringRef attrText = Lexer::getSourceText(charRange, SM, LangOpts);
                if (!attrText.empty() && !attrText.contains("optnone")) {
                    std::string newAttrText;
                    if (attrText.find("()") != StringRef::npos) {
                        // Empty argument list: __attribute__(())
                        newAttrText = "__attribute__((optnone))";
                    } else {
                        // Insert "optnone, " after the second '('.
                        size_t start = attrText.find('(');
                        if (start != StringRef::npos) {
                            start = attrText.find('(', start + 1);
                            if (start != StringRef::npos) {
                                newAttrText = attrText.str();
                                newAttrText.insert(start + 1, "optnone, ");
                            }
                        }
                    }
                    if (!newAttrText.empty() && isRangeSafe(range))
                        Rewriter.ReplaceText(range, newAttrText);
                }
                return; // merged, done.
            }
        }
    }

	// 4. Insert a new attribute after the return type.
	SourceRange retRange = FD->getReturnTypeSourceRange();
	if (retRange.isInvalid()) return;

	SourceLocation insertLoc = Lexer::getLocForEndOfToken(retRange.getEnd(), 0, SM, LangOpts);
	if (insertLoc.isInvalid() || !isLocationSafe(insertLoc)) return;

	Rewriter.InsertText(insertLoc, " __attribute__((optnone))");
}
