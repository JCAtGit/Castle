// expand_macros.cpp
// Compile with: clang++ -std=c++17 expand_macros.cpp -o expand_macros \
//   `llvm-config --cxxflags --ldflags --libs --system-libs` -lclang-cpp

#include "clang/AST/ASTConsumer.h"
#include "clang/Analysis/MacroExpansionContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Lex/PPCallbacks.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "llvm/Support/CommandLine.h"
#include "llvm/Support/raw_ostream.h"
#include <memory>
#include <vector>
#include <string>
#include <optional>

using namespace clang;
using namespace clang::tooling;

static llvm::cl::OptionCategory MacroExpanderCategory("macro-expander");

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

class MacroExpanderAction : public ASTFrontendAction {
public:
  std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI,
                                                 StringRef InFile) override {
    return std::make_unique<ASTConsumer>();
  }

  void ExecuteAction() override {
    CompilerInstance &CI = getCompilerInstance();
    SourceManager &SM = CI.getSourceManager();
    Rewriter.setSourceMgr(SM, CI.getLangOpts());

    MEC = std::make_unique<MacroExpansionContext>(CI.getLangOpts());
    MEC->registerForPreprocessor(CI.getPreprocessor());

    CI.getPreprocessor().addPPCallbacks(
        std::make_unique<MacroExpanderPPCallbacks>(SM, Expansions));

    ASTFrontendAction::ExecuteAction();
  }

  void EndSourceFileAction() override {
      SourceManager &SM = Rewriter.getSourceMgr();
      FileID MainFileID = SM.getMainFileID();

      // Sort expansions by range size (largest first)
      std::sort(Expansions.begin(), Expansions.end(),
          [&SM](const MacroExpansionInfo &A, const MacroExpansionInfo &B) {
              auto lenA = SM.getFileOffset(SM.getExpansionLoc(A.Range.getEnd())) -
                          SM.getFileOffset(SM.getExpansionLoc(A.Range.getBegin()));
              auto lenB = SM.getFileOffset(SM.getExpansionLoc(B.Range.getEnd())) -
                          SM.getFileOffset(SM.getExpansionLoc(B.Range.getBegin()));
              return lenA > lenB;
          });

      struct ReplacedInfo {
          SourceRange Range;
          unsigned StartOffset;
          unsigned EndOffset;
      };
      std::vector<ReplacedInfo> replacedRanges;

      for (const auto &Info : Expansions) {
          unsigned newStart = SM.getFileOffset(SM.getExpansionLoc(Info.Range.getBegin()));
          unsigned newEnd = SM.getFileOffset(SM.getExpansionLoc(Info.Range.getEnd()));
          if (newEnd < newStart) std::swap(newStart, newEnd);

          bool overlaps = false;
          for (const auto &rep : replacedRanges) {
              if (!(newEnd < rep.StartOffset || newStart > rep.EndOffset)) {
                  overlaps = true;
                  break;
              }
          }
          if (overlaps) continue;

          auto ExpandedOpt = MEC->getExpandedText(Info.MacroNameLoc);
          if (ExpandedOpt.has_value()) {
              Rewriter.ReplaceText(Info.Range, ExpandedOpt->str());
              replacedRanges.push_back({Info.Range, newStart, newEnd});
          }
      }

      // Write output file
      std::string OutputFilename = "expanded_output.c";
      std::error_code EC;
      llvm::raw_fd_ostream OS(OutputFilename, EC);
      if (EC) {
          llvm::errs() << "Error opening output file: " << EC.message() << "\n";
          return;
      }

      const llvm::RewriteBuffer *RB = Rewriter.getRewriteBufferFor(MainFileID);
      if (RB) {
          RB->write(OS);
      } else {
          bool Invalid = false;
          StringRef Buf = SM.getBufferData(MainFileID, &Invalid);
          if (!Invalid) OS << Buf;
      }
      OS.flush();
      llvm::errs() << "Expanded source written to: " << OutputFilename << "\n";
  }

private:
  Rewriter Rewriter;
  std::unique_ptr<MacroExpansionContext> MEC;
  std::vector<MacroExpansionInfo> Expansions;
};

int main(int argc, const char **argv) {
  auto ExpectedParser = CommonOptionsParser::create(argc, argv, MacroExpanderCategory);
  if (!ExpectedParser) {
    llvm::errs() << ExpectedParser.takeError();
    return 1;
  }
  CommonOptionsParser &OptionsParser = ExpectedParser.get();
  ClangTool Tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());
  return Tool.run(newFrontendActionFactory<MacroExpanderAction>().get());
}