#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include <vector>
#include "../preprocessor-mechanism/PPCallbacksRecorder.hpp"
#include "CodeEnchanterVisitor.h"
#include "nlohmann/json.hpp"

using namespace clang;
using namespace nlohmann;
using namespace std;

class CodeMarkerAction: public ASTFrontendAction {
	private:
		filesystem::path inputPath, 	inputDir;
		filesystem::path outputPath, 	outputDir;
		filesystem::path path2process;
		json markerFormatCfg;	
		int& idCounter;
		bool& nameEncodedMarkers;
		
		clang::Rewriter rewriter;	
		vector<RecordedPPConstruct> ppEvents; 
		unique_ptr<syntax::TokenCollector> tokenCollector; 
		
	public:
		CodeMarkerAction(
			filesystem::path path2process, 
			filesystem::path inputDir, filesystem::path outputPath,
			filesystem::path outputDir, 
			json& markerFormatCfg, 
			int& idCounter,
			bool& nameEncodedMarkers
		): 
			path2process(path2process), 
			inputDir(inputDir), 
			outputPath(outputPath), 
			outputDir(outputDir), 
			markerFormatCfg(markerFormatCfg), 
			idCounter(idCounter),
			nameEncodedMarkers(nameEncodedMarkers)
			{}
		
		/**
		 * 	Setup van onderdelen:
		 * 	Aanmaak van de ASTConsumer, elk bestand krijgt een eigen instantie van de compiler mee die voor de verwerking zorgt.
		 * 	etc.
		 */
		unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {		
			CI.getDiagnostics().setClient(new clang::IgnoringDiagConsumer(), true);
			CI.getPreprocessor().addPPCallbacks(
				make_unique<PPCallbacksRecorder>(ppEvents, CI.getSourceManager())
			);
			rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
			tokenCollector = make_unique<syntax::TokenCollector>(CI.getPreprocessor());						
			return make_unique<ASTConsumer>();
		}
	
		/**
		 * 	EndSourceFileAction vindt plaats na CreateASTConsumer en de fasen die binnen de consumer plaatsvinden na creatie.
		 * 	Verantwoordelijkheid van file schrijven en wijzigingen aanbrengen mbt markers hoort bij de inserter.
		 */
		void EndSourceFileAction() override {
			CompilerInstance &CI = getCompilerInstance();
	        syntax::TokenBuffer tokenBuffer = std::move(*tokenCollector).consume();
	        tokenBuffer.indexExpandedTokens();
			auto visitor = std::make_unique<CodeMarkerVisitor>(
				CI, 
				rewriter, 
				std::move(tokenBuffer), 
				ppEvents, 
				markerFormatCfg, 
				idCounter,
				nameEncodedMarkers
			);
			visitor->insertMarkerDependency(path2process, inputDir);
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
			if(filesystem::is_regular_file(inputPath)){
				visitor->commitToSingleFile(path2process, outputPath, outputDir);
			} else {
				visitor->commitToQueueFile(inputDir, outputDir);
			}
		}
		
};
