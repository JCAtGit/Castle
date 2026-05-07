#pragma once

#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/FileEntry.h"
#include "clang/AST/Stmt.h"
#include <vector>
#include <string>

#include "../../preprocessor-mechanism/directives/directive-processor.hpp"

using namespace clang;

class RecordingPPCallbacks : public PPCallbacks {
    std::vector<RecordedPPConstruct> &Events;
    SourceManager &SM;

public:
    RecordingPPCallbacks(std::vector<RecordedPPConstruct> &Events, SourceManager &SM)
        : Events(Events), SM(SM) {}

    // Macro expansions
    void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                      SourceRange Range, const MacroArgs *Args) override {
        if (!SM.isInMainFile(Range.getBegin())) return;
        std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
        Events.push_back({Name, Range, true, DirectiveKind::If /*unused*/});
    }

    // Inclusion directive
    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok,
                            StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange,
                            OptionalFileEntryRef File,
                            StringRef SearchPath, StringRef RelativePath,
                            const Module *SuggestedModule, bool ModuleImported,
                            SrcMgr::CharacteristicKind FileType) override {
        SourceRange Range(HashLoc, FilenameRange.getEnd());
        Events.push_back({FileName.str(), Range, false, DirectiveKind::Include});
    }

    // Conditionals
    void If(SourceLocation Loc, SourceRange ConditionRange,
            ConditionValueKind ConditionValue) override {
        Events.push_back({"", ConditionRange, false, DirectiveKind::If});
    }
    void Ifdef(SourceLocation Loc, const Token &MacroNameTok,
               const MacroDefinition &MD) override {
        std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
        SourceRange Range(Loc, MacroNameTok.getLocation());
        Events.push_back({Name, Range, false, DirectiveKind::Ifdef});
    }
    void Ifndef(SourceLocation Loc, const Token &MacroNameTok,
                const MacroDefinition &MD) override {
        std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
        SourceRange Range(Loc, MacroNameTok.getLocation());
        Events.push_back({Name, Range, false, DirectiveKind::Ifndef});
    }
    void Elif(SourceLocation Loc, SourceRange ConditionRange,
              ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
        Events.push_back({"", ConditionRange, false, DirectiveKind::Elif});
    }
    void Else(SourceLocation Loc, SourceLocation IfLoc) override {
        Events.push_back({"", SourceRange(Loc, Loc), false, DirectiveKind::Else});
    }
    void Endif(SourceLocation Loc, SourceLocation IfLoc) override {
        Events.push_back({"", SourceRange(Loc, Loc), false, DirectiveKind::Endif});
    }
    void PragmaDirective(SourceLocation Loc, PragmaIntroducerKind Introducer) override {
        Events.push_back({"", SourceRange(Loc, Loc), false, DirectiveKind::Pragma});
    }
};
