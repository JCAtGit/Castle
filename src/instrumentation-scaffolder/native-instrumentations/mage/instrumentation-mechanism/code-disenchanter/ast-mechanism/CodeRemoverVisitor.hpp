#pragma once

#include "../../../../mage/instrumentation-mechanism/code-disenchanter/CodeMarkerRemover.hpp"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"


using namespace clang;

/**
 * 	TODO:
 *		1. Afhankelijk vd marker flag, remove or insert can hier een verschillende instantie worden meegegeven.
 *		De naam voor de codemarker inserter dus veralgemeniseren.
 */
 class CodeRemoverVisitor: public RecursiveASTVisitor<CodeRemoverVisitor> {
 	private:
 		CodeMarkerRemover& remover;
 	public:
 		explicit CodeRemoverVisitor(CodeMarkerRemover &remover) :remover(remover){}
 
		bool VisitDecl(Decl *D) {
			remover.processDecl(D);
			return true;
		}
	
		bool VisitStmt(Stmt *S) {
			remover.processStmt(S);
			return true;
		}
};
