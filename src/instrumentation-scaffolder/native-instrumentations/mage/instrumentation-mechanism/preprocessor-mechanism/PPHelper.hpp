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
void addOrCreateGNUOptnone(FunctionDecl *FD, Rewriter &Rewriter,
                           const SourceManager &SM, const LangOptions &LangOpts) {
    // Already has optnone? Nothing to do.
    if (FD->hasAttr<OptimizeNoneAttr>())
        return;

    // Conflict: if always_inline is present, skip (or optionally remove it).
    if (FD->hasAttr<AlwaysInlineAttr>() || FD->hasAttr<NakedAttr>() ||
	    FD->hasAttr<WeakAttr>() || FD->hasAttr<WeakRefAttr>())
        return;

    // Get the function body and its opening brace.
    Stmt *Body = FD->getBody();
    if (!Body) return;
    SourceLocation BraceLoc = Body->getBeginLoc();
    if (BraceLoc.isInvalid()) return;

    // Macro‑safety check for a location.
    auto isLocationSafe = [&](SourceLocation Loc) -> bool {
        if (Loc.isInvalid()) return false;
        CharSourceRange TokenRange = CharSourceRange::getTokenRange(Loc, Loc);
        CharSourceRange FileRange = Lexer::makeFileCharRange(TokenRange, SM, LangOpts);
        if (FileRange.isInvalid()) return false;
        return FileRange.getBegin().isValid() && FileRange.getBegin().isFileID();
    };

    // Look for an existing GNU attribute that ends before the brace.
    Attr *TargetAttr = nullptr;
    for (auto *A : FD->attrs()) {
        if (A->getSyntax() != AttributeCommonInfo::AS_GNU)
            continue;
        SourceRange R = A->getRange();
        if (R.isInvalid()) continue;
        if (!isLocationSafe(R.getBegin())) continue;
        if (SM.isBeforeInTranslationUnit(R.getEnd(), BraceLoc)) {
            TargetAttr = A;
            break;
        }
    }

    if (TargetAttr) {
        // We have an existing GNU attribute. We need to merge 'optnone' into its source text.
        // This is where string manipulation is unavoidable because we are rewriting source.
        SourceRange range = TargetAttr->getRange();
        CharSourceRange charRange = Lexer::getAsCharRange(range, SM, LangOpts);
        StringRef attrText = Lexer::getSourceText(charRange, SM, LangOpts);
        if (attrText.empty()) return;

        // Double‑check that the attribute doesn't already have optnone (just in case).
        if (attrText.contains("optnone")) return;

        // Build the new attribute text by inserting "optnone, " into the argument list.
        std::string newAttrText;
        if (attrText.find("()") != StringRef::npos) {
            newAttrText = "__attribute__((optnone))";
        } else {
            size_t start = attrText.find('(');
            if (start != StringRef::npos) {
                start = attrText.find('(', start + 1);
                if (start != StringRef::npos) {
                    newAttrText = attrText.str();
                    newAttrText.insert(start + 1, "optnone, ");
                }
            }
        }
        if (!newAttrText.empty() && isLocationSafe(range.getBegin())) {
            Rewriter.ReplaceText(range, newAttrText);
        }
        return;
    }

    // No existing attribute before the brace: insert a new one.
    if (!isLocationSafe(BraceLoc))
        return;
    Rewriter.InsertText(BraceLoc, " __attribute__((optnone)) ");
}

bool isLocationSafeForInsertion(SourceLocation Loc,
                                const SourceManager &SM,
                                const LangOptions &LangOpts) {
    if (Loc.isInvalid())
        return false;

    // Create a single‑token range at this location.
    CharSourceRange TokenRange = CharSourceRange::getTokenRange(Loc, Loc);

    // Attempt to obtain the file character range.
    CharSourceRange FileRange = Lexer::makeFileCharRange(TokenRange, SM, LangOpts);

    // If the range is invalid, the location is inside a macro expansion,
    // a macro argument, or otherwise not representable as a simple file range.
    if (FileRange.isInvalid())
        return false;

    // Ensure the resulting location is actually in a file (not a buffer).
    SourceLocation Begin = FileRange.getBegin();
    if (Begin.isInvalid() || !Begin.isFileID())
        return false;

    return true;
}
