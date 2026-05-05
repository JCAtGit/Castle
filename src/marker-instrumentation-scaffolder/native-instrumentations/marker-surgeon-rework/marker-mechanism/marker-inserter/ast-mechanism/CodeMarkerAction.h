#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"
#include <memory>
#include <vector>

#include "../CodeMarkerInserter.hpp"
#include "../../../config/config_ds.hpp"
#include "RecordingPPCallbacks.hpp"
#include "CodeMarkerVisitor.h"

using namespace clang;

class CodeMarkerAction: public ASTFrontendAction {
	private:
		clang::Rewriter Rewriter;
		std::unique_ptr<CodeMarkerInserter> CMI; // kan aanwezig zijn voor de eind declaratie in EndSourceFileAction, maar kan ook als actie in handleTranslationUnit.
		struct config& cfg;
		vector<RecordedPPConstruct> ppEvents; 
		std::unique_ptr<syntax::TokenCollector> tokenCollector; 
		
	public:
		CodeMarkerAction(struct config& cfg): cfg(cfg) {}
		
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
			Rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());
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
			
			CMI = std::make_unique<CodeMarkerInserter>(CI.getASTContext(), Rewriter, cfg, ppEvents);
			CMI->setTokenBuffer(std::move(tokenBuffer));

			auto visitor = std::make_unique<CodeMarkerVisitor>(*CMI);
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
			CMI->commitToFile();
		}
};
