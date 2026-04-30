
#pragma once

#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/Tooling/Syntax/Tokens.h"

#include <vector>
#include <string>
#include <optional>
#include <algorithm>

using namespace clang;
// --------------------------------------------------------------
// Data structures for recorded preprocessor events
// --------------------------------------------------------------
//clang::tok::PPKeywordKind
enum class DirectiveKind {
    Include, If, Ifdef, Ifndef, Elif, Else, Endif, Pragma
};

struct RecordedPPConstruct {
    std::string name;       // macro name, filename, or empty for bare directives
    SourceRange range;
    bool isMacro;           // true for macro expansions, false for other directives
    DirectiveKind kind;     // meaningful only if !isMacro
};

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
// Generalised detection
// --------------------------------------------------------------
struct PPEventVectors {
    std::vector<RecordedPPConstruct> before;      // event ends exactly before statement
    std::vector<RecordedPPConstruct> after;       // event starts exactly after statement
    std::vector<RecordedPPConstruct> containing;  // event encloses statement
    std::vector<RecordedPPConstruct> inside;      // event is inside statement
};

PPEventVectors detectPPConstructsAroundStatement(
    const Stmt *S, const syntax::TokenBuffer &Tokens,
    const SourceManager &SM, const std::vector<RecordedPPConstruct> &Events) {

    PPEventVectors result;
    if (!S) return result;
    SourceRange stmtRange = S->getSourceRange();
    if (stmtRange.isInvalid()) return result;

    // Get spelled tokens for main file
    FileID mainFileID = SM.getMainFileID();
    auto allSpelled = Tokens.spelledTokens(mainFileID);
    if (allSpelled.empty()) return result;

    // Find token indices for the statement
    size_t firstStmtIdx = allSpelled.size();
    size_t lastStmtIdx = 0;
    for (size_t i = 0; i < allSpelled.size(); ++i) {
        if (SM.isPointWithin(allSpelled[i].location(), stmtRange.getBegin(), stmtRange.getEnd())) {
            firstStmtIdx = std::min(firstStmtIdx, i);
            lastStmtIdx = i;
        }
    }
    if (firstStmtIdx > lastStmtIdx) return result;

    // Process each recorded event
    for (const auto &ev : Events) {
        auto tokRange = getTokenIndicesForRange(ev.range, allSpelled, SM);
        if (!tokRange) continue;
        size_t firstEvtIdx = tokRange->firstIdx;
        size_t lastEvtIdx = tokRange->lastIdx;

        if (firstEvtIdx <= firstStmtIdx && lastEvtIdx >= lastStmtIdx) {
            result.containing.push_back(ev);
        } else if (firstEvtIdx >= firstStmtIdx && lastEvtIdx <= lastStmtIdx) {
            result.inside.push_back(ev);
        } else if (lastEvtIdx + 1 == firstStmtIdx) {
            result.before.push_back(ev);
        } else if (lastStmtIdx + 1 == firstEvtIdx) {
            result.after.push_back(ev);
        }
    }
    return result;
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
