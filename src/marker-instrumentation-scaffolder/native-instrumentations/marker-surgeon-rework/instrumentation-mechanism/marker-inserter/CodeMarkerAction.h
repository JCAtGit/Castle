#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"
#include <memory>
#include <vector>
#include "nlohmann/json.hpp"

#include "RecordingPPCallbacks.hpp"
#include "CodeMarkerVisitor.h"

using namespace clang;
using namespace nlohmann;
using namespace std;

class CodeMarkerAction: public ASTFrontendAction {
	private:
		clang::Rewriter rewriter;
		json markerFormatCfg;
		vector<RecordedPPConstruct> ppEvents; 
		std::unique_ptr<syntax::TokenCollector> tokenCollector; 
		
		
		// TODO: Generalize and change
		void insertMarkerDependency(){
			SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
			FileID mainFileID = SM.getMainFileID();
			StringRef filePath = SM.getFilename(startLoc);
			rewriter.InsertTextBefore(
				startLoc,
				icp->synthesizeMarkerDependencyString(filePath.str(), &cfg)
			);
		}
		
		void commitToFile(){
			llvm::outs() << "Start commit phase" << "\n";
			insertMarkerDependency();			

			// returns empty if path is on same dir.
			FileID MainFileID = SM.getMainFileID();
			StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(MainFileID));

			fs::path outPath = cfg.outputDir / rel_outputPath(cfg.inputDir, FilePath.str());
			if (outPath.empty() || outPath == "." || outPath == "./") {
			    outPath = FilePath.str();
			}
			llvm::outs() << "Committing changes from: " << FilePath << " -> " << outPath << "\n";
			// committing buffered changes to file
			if(!std::filesystem::exists(outPath.string())){
				fs::create_directories(outPath.parent_path());
			}
			// write to file and mutate with srcExt.
			std::error_code EC;
			llvm::raw_fd_ostream OutFile(
				fs::path(outPath).replace_filename(outPath.stem().string() + cfg.srcExt + outPath.extension().string()).string(),
				EC, 
				llvm::sys::fs::OF_Text
			);

			if (EC) {
				llvm::errs() << "Error opening output file: " << EC.message()<< "\n";
				return;
			}

			// Write buffered changes to file.
			rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
			OutFile.close();	
		}
		
		static void createDependencyFiles(struct config& cfg){
			switch(cfg.format){
				default: ICP0::createDependencyFiles(cfg); break;
				case 1:  ICP1::createDependencyFiles(cfg); break;
			}
		}
		
		
	public:
		CodeMarkerAction(json markerFormatCfg): markerFormatCfg(markerFormatCfg) {}
		
		/**
		 * 	Setup van onderdelen:
		 * 	Aanmaak van de ASTConsumer, elk bestand krijgt een eigen instantie van de compiler mee die voor de verwerking zorgt.
		 * 	etc.
		 */
		std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {		
//			CI.getPreprocessor().SetSuppressIncludeNotFoundError(true);
			CI.getDiagnostics().setClient(new clang::IgnoringDiagConsumer(), true);
			CI.getPreprocessor().addPPCallbacks(
				std::make_unique<RecordingPPCallbacks>(ppEvents, CI.getSourceManager())
			);

			// Set source for writestream to insert codemarkers into.
			rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
			tokenCollector = std::make_unique<syntax::TokenCollector>(CI.getPreprocessor());						
			return std::make_unique<ASTConsumer>();
		}
	
		/**
		 * 	EndSourceFileAction vindt plaats na CreateASTConsumer en de fasen die binnen de consumer plaatsvinden na creatie.
		 * 	Verantwoordelijkheid van file schrijven en wijzigingen aanbrengen mbt markers hoort bij de inserter.
		 */
		void EndSourceFileAction() override {
			CompilerInstance &CI = getCompilerInstance();
	        syntax::TokenBuffer tokenBuffer = std::move(*tokenCollector).consume();
	        tokenBuffer.indexExpandedTokens();
			
			auto visitor = std::make_unique<CodeMarkerVisitor>(tokenBuffer, ppEvents, markerFormatCfg);
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
			commitToFile();
		}
};
