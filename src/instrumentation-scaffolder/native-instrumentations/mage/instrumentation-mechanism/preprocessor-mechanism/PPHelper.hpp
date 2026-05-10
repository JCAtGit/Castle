#pragma once

#include "clang/AST/DeclBase.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/Stmt.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <vector>
#include <string>
#include <optional>
#include <algorithm>
#include "PPDatastructures.hpp"

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


