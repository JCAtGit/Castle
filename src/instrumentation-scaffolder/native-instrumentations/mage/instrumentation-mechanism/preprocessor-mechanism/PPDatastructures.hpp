#pragma once
#include <string>
#include "clang/Basic/SourceManager.h"

using namespace clang;
using namespace std;

/**
 *	Data structures used for recording preprocessor events and holding preprocessor events (primarily source ranges).
 */
 

enum class DirectiveKind {
    Include, If, Ifdef, Ifndef, Elif, Else, Endif, Pragma
};

struct RecordedPPConstruct {
    string name;       // macro name, filename, or empty for bare directives
    SourceRange range;
    bool isMacro;           // true for macro expansions, false for other directives
    DirectiveKind kind;     // meaningful only if !isMacro
};

struct PPEventVectors {
    vector<RecordedPPConstruct> before;      // event ends exactly before statement
    vector<RecordedPPConstruct> after;       // event starts exactly after statement
    vector<RecordedPPConstruct> containing;  // event encloses statement
    vector<RecordedPPConstruct> inside;      // event is inside statement
};
