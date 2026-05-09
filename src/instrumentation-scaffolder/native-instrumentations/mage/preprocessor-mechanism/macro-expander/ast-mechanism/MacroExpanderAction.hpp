#pragma once

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

#include "../../../../mage/preprocessor-mechanism/macro-expander/ast-mechanism/MacroExpanderPPCallbacks.hpp"
#include "../../../config/config.hpp"
using namespace clang;
using namespace clang::tooling;


class MacroExpanderAction: public ASTFrontendAction {
	private:
	  	Rewriter Rewriter;
	  	std::unique_ptr<MacroExpansionContext> MEC;
	  	std::vector<MacroExpansionInfo> Expansions;
		std::string path2process;
		config& cfg;

	public:
		MacroExpanderAction(std::string path2process, config& cfg):path2process(path2process), cfg(cfg) {}
	
		std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
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
	    std::string OutputFilename = path2process;
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
};