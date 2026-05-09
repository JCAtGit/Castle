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
		
	public:
		CodeMarkerAction(json& markerFormatCfg): markerFormatCfg(markerFormatCfg) {}
		
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
			
			auto visitor = std::make_unique<CodeMarkerVisitor>(CI.getASTContext(), &rewriter, std::move(tokenBuffer), ppEvents, markerFormatCfg);
			visitor->insertMarkerDependency();
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
			visitor->commitToFile();
		}
};
