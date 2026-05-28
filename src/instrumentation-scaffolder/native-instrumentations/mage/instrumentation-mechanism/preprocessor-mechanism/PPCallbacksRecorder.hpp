#pragma once

#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Preprocessor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Basic/FileEntry.h"
#include "clang/AST/Stmt.h"
#include <vector>
#include <string>
#include "PPDatastructures.hpp"

using namespace clang;

class PPCallbacksRecorder : public PPCallbacks {
    std::vector<RecordedPPConstruct> &Events;
    SourceManager &SM;

public:
    PPCallbacksRecorder(std::vector<RecordedPPConstruct> &Events, SourceManager &SM): Events(Events), SM(SM) {}

    void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD, SourceRange Range, const MacroArgs *Args) override {
        if (!SM.isInMainFile(Range.getBegin())) return;
        Events.push_back({MacroNameTok.getIdentifierInfo()->getName().str(), Range, true, DirectiveKind::If /*unused*/});
    }

    void InclusionDirective(SourceLocation HashLoc, const Token &IncludeTok, StringRef FileName, bool IsAngled,
                            CharSourceRange FilenameRange, OptionalFileEntryRef File, StringRef SearchPath, 
							StringRef RelativePath, const Module *SuggestedModule, bool ModuleImported,
                            SrcMgr::CharacteristicKind FileType) override {
        Events.push_back({FileName.str(), SourceRange(HashLoc, FilenameRange.getEnd()), false, DirectiveKind::Include});
    }

    void If(SourceLocation Loc, SourceRange ConditionRange, ConditionValueKind ConditionValue) override {
        Events.push_back({"", ConditionRange, false, DirectiveKind::If});
    }
    void Ifdef(SourceLocation Loc, const Token &MacroNameTok, const MacroDefinition &MD) override {
        std::string Name = MacroNameTok.getIdentifierInfo()->getName().str();
        Events.push_back({Name, SourceRange(Loc, MacroNameTok.getLocation()), false, DirectiveKind::Ifdef});
    }
    void Ifndef(SourceLocation Loc, const Token &MacroNameTok, const MacroDefinition &MD) override {
        Events.push_back({MacroNameTok.getIdentifierInfo()->getName().str(), SourceRange(Loc, MacroNameTok.getLocation()), false, DirectiveKind::Ifndef});
    }
    void Elif(SourceLocation Loc, SourceRange ConditionRange, ConditionValueKind ConditionValue, SourceLocation IfLoc) override {
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
