#pragma once

#include <vector>
#include <algorithm>
#include "clang/Lex/PPCallbacks.h"
#include "clang/Basic/SourceManager.h"

struct MacroInterval {
    unsigned start;   		// inclusive file offset of the macro invocation's first char
    unsigned end;     		// inclusive file offset of the macro invocation's last char
};

/**
 *	Records the range of Macro's upon callback from the preprocessor.
 * 	The recorded ranges are then used to compare the overlap between a statement range and potential macro ranges in its vicinity.
 *	Because of this callback pattern, we must ensure sort status during each visit.
 */
class MacroExpansionRecorder : public clang::PPCallbacks {
    clang::SourceManager &SM;
    std::vector<MacroInterval> intervals;
    bool sorted = false;	// flag to ensure sort before queries

	public:
    MacroExpansionRecorder(clang::SourceManager &SM) : SM(SM) {}

    // Record only macro expansions (the only thing we care about) 
    void MacroExpands(
		const clang::Token &MacroNameTok,
        const clang::MacroDefinition &MD,
        clang::SourceRange Range,
        const clang::MacroArgs *Args
	) override {
        // Only care about expansions in the main file (adjust if needed)
        if (!SM.isInMainFile(Range.getBegin())) return;

        clang::SourceLocation begin = SM.getSpellingLoc(Range.getBegin());
        clang::SourceLocation end   = SM.getSpellingLoc(Range.getEnd());
        if (!begin.isValid() || !end.isValid()) return;

        intervals.push_back({
            SM.getFileOffset(begin),
            SM.getFileOffset(end)   // Range is inclusive, so end is the last char
        });
        sorted = false;
    }

	/**
	 *	Sorts macro intervals on their starting values.
	 */
    void sortIntervals() {
        if (!sorted) {
            std::sort(intervals.begin(), intervals.end(),
                [](const MacroInterval& a, const MacroInterval& b) {
                    return a.start < b.start;
                });
            sorted = true;
        }
    }

	/**
	 *	Check for macro overlap, given some stmt range.
	 * 	@param	stmtRange	The range of a given entity
	 *	@return				True if macro range has overlap with entity range, false otherwise
	 */
    bool hasOverlap(clang::SourceRange stmtRange) const {
        if (!sorted) {const_cast<MacroExpansionRecorder*>(this)->sortIntervals();} // see class snippet

        clang::SourceLocation stmtBegin = SM.getSpellingLoc(stmtRange.getBegin());
        clang::SourceLocation stmtEnd   = SM.getSpellingLoc(stmtRange.getEnd());
        if (!stmtBegin.isValid() || !stmtEnd.isValid()) return false;

        unsigned stmtStart = SM.getFileOffset(stmtBegin);
        unsigned stmtEndOffset = SM.getFileOffset(stmtEnd);   // inclusive

        // Find first interval with start >= stmtStart
        auto it = std::lower_bound(
			intervals.begin(), 
			intervals.end(), 
			stmtStart,
            [](const MacroInterval& interval, unsigned val) {
                return interval.start < val;
            }
		);

        // Scan all intervals whose start lies within the statement range
        for (; it != intervals.end() && it->start <= stmtEndOffset; it++) {
            if (it->end >= stmtStart)   // overlap condition
                return true;
        }
        return false;
    }

	/**
	 *	Checks if any intervals exists before some potential insertion point for macro overlap.
	 *	@param	offset	source location index of some AST construct.
	 *	@return			true, if there is overlap of a macro with the insertion point, false otherwise.
	 */
	bool isOffsetInsideMacro(unsigned offset) const {
	    if (!sorted) const_cast<MacroExpansionRecorder*>(this)->sortIntervals(); // see class snippet

	    // Find the first interval with start > offset
	    auto it = std::upper_bound(intervals.begin(), intervals.end(), offset,
	        [](unsigned val, const MacroInterval& m) { return val < m.start; });

	    // If it's the first, no interval starts before or at offset
	    if (it == intervals.begin()) return false;

	    // Check the interval just before (its start <= offset)
	    --it;
	    return offset <= it->end;   // true if offset is inside that macro
	}

	/**
	 *	Gives access to raw intervals (if needed)
	 */
    const std::vector<MacroInterval>& getIntervals() const {return intervals;}
};

