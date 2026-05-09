#pragma once

#include "clang/Lex/PPCallbacks.h"
#include "clang/Lex/Token.h"

using namespace clang;

struct MacroExpansionInfo {
  SourceRange Range;
  SourceLocation MacroNameLoc;
};

class MacroExpanderPPCallbacks : public PPCallbacks {
public:
  MacroExpanderPPCallbacks(SourceManager &SM, std::vector<MacroExpansionInfo> &Expansions)
      : SM(SM), Expansions(Expansions) {}

  void MacroExpands(const Token &MacroNameTok, const MacroDefinition &MD,
                    SourceRange Range, const MacroArgs *Args) override {
    if (SM.isInMainFile(Range.getBegin())) {
      Expansions.push_back({Range, MacroNameTok.getLocation()});
    }
  }

private:
  SourceManager &SM;
  std::vector<MacroExpansionInfo> &Expansions;
};