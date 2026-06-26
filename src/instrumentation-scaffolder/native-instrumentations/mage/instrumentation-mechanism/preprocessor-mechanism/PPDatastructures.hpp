#pragma once
#include <string>
#include "clang/Basic/SourceManager.h"

using namespace clang;

/**
 *	Data structures for recorded preprocessor events
 TODO: use clang::tok::PPKeywordKind instead of DirectiveKind?
 */
enum class DirectiveKind {
    Include, If, Ifdef, Ifndef, Elif, Else, Endif, Pragma
};

struct RecordedPPConstruct {
    std::string name;       // macro name, filename, or empty for bare directives
    SourceRange range;
    bool isMacro;           // true for macro expansions, false for other directives
    DirectiveKind kind;     // meaningful only if !isMacro
};

struct PPEventVectors {
    std::vector<RecordedPPConstruct> before;      // event ends exactly before statement
    std::vector<RecordedPPConstruct> after;       // event starts exactly after statement
    std::vector<RecordedPPConstruct> containing;  // event encloses statement
    std::vector<RecordedPPConstruct> inside;      // event is inside statement
};
