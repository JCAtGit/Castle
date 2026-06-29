#pragma once

#include <vector>
#include <algorithm>
#include "clang/Lex/PPCallbacks.h"
#include "clang/Basic/SourceManager.h"

struct MacroInterval {
    unsigned start;   // inclusive file offset of the macro invocation's first char
    unsigned end;     // inclusive file offset of the macro invocation's last char
};

class MacroExpansionRecorder : public clang::PPCallbacks {
    clang::SourceManager &SM;
    std::vector<MacroInterval> intervals;
    bool sorted = false;   // flag to ensure sort before queries

public:
    MacroExpansionRecorder(clang::SourceManager &SM) : SM(SM) {}

    // ---- Record only macro expansions (the only thing we care about) ----
    void MacroExpands(const clang::Token &MacroNameTok,
                      const clang::MacroDefinition &MD,
                      clang::SourceRange Range,
                      const clang::MacroArgs *Args) override {
        // Only care about expansions in the main file (adjust if needed)
        if (!SM.isInMainFile(Range.getBegin()))
            return;

        clang::SourceLocation begin = SM.getSpellingLoc(Range.getBegin());
        clang::SourceLocation end   = SM.getSpellingLoc(Range.getEnd());
        if (!begin.isValid() || !end.isValid())
            return;

        intervals.push_back({
            SM.getFileOffset(begin),
            SM.getFileOffset(end)   // Range is inclusive, so end is the last char
        });
        sorted = false;
    }

    // ---- Finalize: sort intervals once after all callbacks ----
    void sortIntervals() {
        if (!sorted) {
            std::sort(intervals.begin(), intervals.end(),
                [](const MacroInterval& a, const MacroInterval& b) {
                    return a.start < b.start;
                });
            sorted = true;
        }
    }

    // ---- Query: does any macro overlap the given source range? ----
    bool hasOverlap(clang::SourceRange stmtRange) const {
        if (!sorted) {
            // Force sort (or you can call sortIntervals() before queries)
            const_cast<MacroExpansionRecorder*>(this)->sortIntervals();
        }

        clang::SourceLocation stmtBegin = SM.getSpellingLoc(stmtRange.getBegin());
        clang::SourceLocation stmtEnd   = SM.getSpellingLoc(stmtRange.getEnd());
        if (!stmtBegin.isValid() || !stmtEnd.isValid())
            return false;

        unsigned stmtStart = SM.getFileOffset(stmtBegin);
        unsigned stmtEndOffset = SM.getFileOffset(stmtEnd);   // inclusive

        // Find first interval with start >= stmtStart
        auto it = std::lower_bound(intervals.begin(), intervals.end(), stmtStart,
            [](const MacroInterval& interval, unsigned val) {
                return interval.start < val;
            });

        // Scan all intervals whose start lies within the statement range
        for (; it != intervals.end() && it->start <= stmtEndOffset; ++it) {
            if (it->end >= stmtStart)   // overlap condition
                return true;
        }
        return false;
    }

    // ---- Optional: query a single offset (for injection at a point) ----
    bool isPointInsideMacro(unsigned offset) const {
        if (!sorted) {
            const_cast<MacroExpansionRecorder*>(this)->sortIntervals();
        }

        auto it = std::upper_bound(intervals.begin(), intervals.end(), offset,
            [](unsigned val, const MacroInterval& interval) {
                return val < interval.start;
            });

        // Check previous interval (if any) for containment
        if (it != intervals.begin()) {
            --it;
            if (it->start <= offset && offset <= it->end)
                return true;
        }
        // Also check the next interval if it starts exactly at offset
        // (this covers injection right before a macro begins)
        if (it != intervals.end() && it->start == offset)
            return true;
        return false;
    }

    // ---- Access to raw intervals (if needed) ----
    const std::vector<MacroInterval>& getIntervals() const { return intervals; }
};

