#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Lex/Preprocessor.h"
#include <memory>
#include <vector>
#include "EvalVisitor.hpp"
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace clang;

class EvalAction: public ASTFrontendAction {
	private:
		json cfg;
		
	public:
		EvalAction(json cfg): cfg(cfg) {}
		
		/**
		 * 	Setup van onderdelen:
		 * 	Aanmaak van de ASTConsumer, elk bestand krijgt een eigen instantie van de compiler mee die voor de verwerking zorgt.
		 * 	etc.
		 */
		std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {		
//			CI.getPreprocessor().SetSuppressIncludeNotFoundError(true);
			CI.getDiagnostics().setClient(new clang::IgnoringDiagConsumer(), true);
			return std::make_unique<ASTConsumer>();
		}
	
		/**
		 * 	EndSourceFileAction vindt plaats na CreateASTConsumer en de fasen die binnen de consumer plaatsvinden na creatie.
		 * 	Verantwoordelijkheid van file schrijven en wijzigingen aanbrengen mbt markers hoort bij de inserter.
		 */
		void EndSourceFileAction() override {
			auto visitor = std::make_unique<EvalVisitor>();
			visitor->TraverseDecl(getCompilerInstance().getASTContext().getTranslationUnitDecl());
		}
};
