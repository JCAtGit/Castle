#pragma once

#include "../../../../../../prototype/marker-surgeon/config/config_ds.hpp"
#include "../../../../mage/instrumentation-mechanism/code-disenchanter/ast-mechanism/CodeRemoverVisitor.hpp"
#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"

#include "../../code-disenchanter/CodeMarkerRemover.hpp"

using namespace clang;

class CodeRemoverAction: public ASTFrontendAction {
	private:
		clang::Rewriter Rewriter;
		std::unique_ptr<CodeMarkerRemover> CMR; // kan aanwezig zijn voor de eind declaratie in EndSourceFileAction, maar kan ook als actie in handleTranslationUnit.
		struct config& cfg;
	public:
	CodeRemoverAction(struct config& cfg): cfg(cfg) {}

		std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {		
			// Suppress warnings
			CI.getPreprocessor().SetSuppressIncludeNotFoundError(true);
			// Set a diagnostic client that ignores all clang diagnostics
			CI.getDiagnostics().setClient(new clang::IgnoringDiagConsumer());
			// Set source for writestream to insert codemarkers into.
			Rewriter.setSourceMgr(CI.getSourceManager(), CI.getLangOpts());

			CMR = std::make_unique<CodeMarkerRemover>(CI.getASTContext(), Rewriter, cfg);
			return std::make_unique<ASTConsumer>();
		}
	
		void EndSourceFileAction() override {
			CompilerInstance &CI = getCompilerInstance();
			auto visitor = std::make_unique<CodeRemoverVisitor>(*CMR);
			visitor->TraverseDecl(CI.getASTContext().getTranslationUnitDecl());
			CMR->removeMarkerHeaderInclusion(); // TODO
			CMR->commitToFile();
		}
};
