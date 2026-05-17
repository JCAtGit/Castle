#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <nlohmann/json.hpp>

using namespace nlohmann;
using namespace clang;
using namespace std;

class OracleSetupAction: public ASTFrontendAction {   
	private:
		
		/** 
		 * 	Stel recovery options in voordat verwerking en gerelateerde tools plaatsvinden.
		 * 	ASTFrontendAction, call order is:
	   	 * 		PrepareToExecuteAction (called by Execute)
		 * 		BeginSourceFileAction ← set LangOptions and add callbacks
		 *		CreateASTConsumer
		 */
		bool BeginSourceFileAction(CompilerInstance &CI) override {
	        auto &langOpts = CI.getLangOpts();
	        langOpts.RecoveryAST = true;
	        langOpts.RecoveryASTType = true;
	        CI.getDiagnostics().setClient(new IgnoringDiagConsumer(), true);
	        // CI.getPreprocessor().SetSuppressIncludeNotFoundError(true); // uncomment if needed
			return true;
	    }

		unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef InFile) override {
			return make_unique<ASTConsumer>();
		}
		
		/**
		 * 	EndSourceFileAction vindt plaats na CreateASTConsumer en de fasen die binnen de consumer plaatsvinden na creatie.
		 * 	Verantwoordelijkheid van file schrijven en wijzigingen aanbrengen mbt markers hoort bij de inserter.
		 */
		void EndSourceFileAction() override {}
		
	public:
	    OracleSetupAction() {}
};
