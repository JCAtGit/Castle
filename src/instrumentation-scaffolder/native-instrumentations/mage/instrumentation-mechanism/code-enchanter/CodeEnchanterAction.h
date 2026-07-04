#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <vector>
#include "../preprocessor/MacroExpansionRecorder.hpp"
#include "CodeEnchanterVisitor.h"
#include "nlohmann/json.hpp"

using namespace clang;
using namespace nlohmann;
using namespace std;

class CodeEnchanterAction: public ASTFrontendAction {
	private:
		filesystem::path 			inputPath, 	inputDir,
									outputPath, outputDir,
									path2process;
									
		json 						markerFormatCfg;	
		int							&idCounter;
		bool						&nameEncodedMarker, clangFormatMode;
		
		clang::Rewriter 			rewriter;
		
		MacroExpansionRecorder* 	macroRecorder = nullptr;
		
	public:
		CodeEnchanterAction(
			filesystem::path path2process, 
			filesystem::path inputDir, 
			filesystem::path outputPath,
			filesystem::path outputDir, 
			json& markerFormatCfg, 
			int& idCounter,
			bool& nameEncodedMarker,
			bool& formatMode
		): 
			path2process(path2process), 
			inputDir(inputDir), 
			outputPath(outputPath), 
			outputDir(outputDir), 
			markerFormatCfg(markerFormatCfg), 
			idCounter(idCounter),
			nameEncodedMarker(nameEncodedMarker),
			clangFormatMode(formatMode)
			{}
		
		unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {		
			CI.getDiagnostics().setClient(new clang::IgnoringDiagConsumer(), true);
			
			auto recorder = std::make_unique<MacroExpansionRecorder>(CI.getSourceManager());
			macroRecorder = recorder.get();
			CI.getPreprocessor().addPPCallbacks(
				std::move(recorder)
			);
						
			rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
			return make_unique<ASTConsumer>();
		}
	
		/**
		 * 	EndSourceFileAction takes place after CreateASTConsumer.
		 */
		void EndSourceFileAction() override {
			CompilerInstance &CI = getCompilerInstance();
			auto visitor = std::make_unique<CodeEnchanterVisitor>(
				CI, 
				rewriter, 
				macroRecorder,
				markerFormatCfg, 
				idCounter,
				nameEncodedMarker,
				clangFormatMode
			);
			
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
			if(nameEncodedMarker){
				// https://github.com/isocpp/CppCoreGuidelines/blob/master/CppCoreGuidelines.md#Rs-include-order
				// But we only use this in ".c" files, not headers.
				visitor->insertNameEncodedMarkerDefinitions();
			}
			visitor->insertPreprocessorDirectives(path2process, inputDir);

			if(filesystem::is_regular_file(inputPath)){
				visitor->commitToSingleFile(outputPath);
			} else {
				visitor->commitToQueueFile(inputDir, outputDir);
			}
		}
};

