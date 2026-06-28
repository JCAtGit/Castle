#pragma once

#include "clang/Basic/SourceManager.h"
#include <vector>
#include "PPDatastructures.hpp"
#include "PPHelper.hpp"

/**
 *	Uses the recorded callback events to check if the surrounding context allows for instrumentation.
 */
 
PPEventVectors detectPPConstructsAroundDeclaration(
    const Decl *D,
    const syntax::TokenBuffer &Tokens,
    const SourceManager &SM,
    const std::vector<RecordedPPConstruct> &Events) {

    PPEventVectors result;
    if (!D) return result;

    SourceRange declRange = D->getSourceRange();
    if (declRange.isInvalid()) return result;

    // Get spelled tokens for the main file
    FileID mainFileID = SM.getMainFileID();
    auto allSpelled = Tokens.spelledTokens(mainFileID);
    if (allSpelled.empty()) return result;

    // Find token indices that cover the declaration's source range
    size_t firstDeclIdx = allSpelled.size();
    size_t lastDeclIdx = 0;
    for (size_t i = 0; i < allSpelled.size(); ++i) {
        SourceLocation loc = allSpelled[i].location();
        if (SM.isPointWithin(loc, declRange.getBegin(), declRange.getEnd())) {
            firstDeclIdx = std::min(firstDeclIdx, i);
            lastDeclIdx = i;
        }
    }
    if (firstDeclIdx > lastDeclIdx) return result; // No tokens found

    // Classify each recorded event
    for (const auto &ev : Events) {
        auto tokRange = getTokenIndicesForRange(ev.range, allSpelled, SM);
        if (!tokRange) continue;

        size_t firstEvtIdx = tokRange->firstIdx;
        size_t lastEvtIdx = tokRange->lastIdx;

        if (firstEvtIdx <= firstDeclIdx && lastEvtIdx >= lastDeclIdx) {
            result.containing.push_back(ev);          // Event fully encloses declaration
        } else if (firstEvtIdx >= firstDeclIdx && lastEvtIdx <= lastDeclIdx) {
            result.inside.push_back(ev);              // Event is inside declaration
        } else if (lastEvtIdx + 1 == firstDeclIdx) {
            result.before.push_back(ev);              // Event ends immediately before declaration
        } else if (lastDeclIdx + 1 == firstEvtIdx) {
            result.after.push_back(ev);               // Event starts immediately after declaration
        }
    }
    return result;
}

