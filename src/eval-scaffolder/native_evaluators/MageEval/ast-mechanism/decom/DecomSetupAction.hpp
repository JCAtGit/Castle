#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/Tooling.h"
#include <memory>
#include <nlohmann/json.hpp>

#include "DecomSetupVisitor.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class DecomSetupAction: public ASTFrontendAction {   
	private:
		json &markerFormat;
		unordered_map<string, FunctionDecl*> decomASTFnDeclIndex;
		unordered_map<int, CallExpr*> decomASTCallExprIndex;
		unique_ptr<DecomSetupVisitor> dsVisitor;
		
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
		void EndSourceFileAction() override {
			dsVisitor = std::make_unique<DecomSetupVisitor>(
				markerFormat,
				getCompilerInstance().getASTContext(),
				decomASTFnDeclIndex,
				decomASTCallExprIndex
			);
			dsVisitor->TraverseDecl(getCompilerInstance().getASTContext().getTranslationUnitDecl());
		}
		
	public:
	    // Constructor takes shared ownership of the CompilerInstance
	    DecomSetupAction(json& markerFormat) : markerFormat(markerFormat) {}
		
		unordered_map<string, FunctionDecl*>& getDecomASTFnDeclIndex(){
			return decomASTFnDeclIndex;
		}
		
		unordered_map<int, CallExpr*>& getDecomASTCallExprIndex(){
			return decomASTCallExprIndex;
		}
};
