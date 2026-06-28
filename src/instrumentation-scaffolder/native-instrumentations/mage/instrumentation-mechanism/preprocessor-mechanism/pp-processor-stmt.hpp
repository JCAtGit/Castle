#pragma once

#include "clang/AST/Stmt.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <vector>
#include "PPDatastructures.hpp"
#include "PPHelper.hpp"

using namespace clang;

/**
 *	Uses the recorded callback events to check if the surrounding context allows for instrumentation.
 */

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
