#pragma once

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"
#include <optional>

#include "../../../../../../../prototype/marker-surgeon/config/config_ds.hpp"
#include "../../../../../include/helper.hpp"
#include "../../../code-disenchanter/construct-processors/RemoverConstructProcessorInterface.hpp"

using namespace clang;
using namespace std;

class RCP0: public RemoverConstructProcessor {
	private:
		ASTContext* context;
		Rewriter* R; 
		string markerName;

	public:
	RCP0(
		ASTContext* context, 
		Rewriter* R, 
		std::string markerName
	) : context(context), R(R), markerName(markerName){}

	
	template<typename NodeType> // only decl and stmt
	void deleteNode(NodeType* ASTNode) {
		SourceRange range = ASTNode->getSourceRange();
//		SourceLocation end;
		SourceLocation semicolon = Lexer::findLocationAfterToken(range.getEnd(), tok::semi, context->getSourceManager(), context->getLangOpts(), false);
		if (semicolon.isValid()) {
			SourceLocation afterSemi = Lexer::getLocForEndOfToken(semicolon, 0, context->getSourceManager(), context->getLangOpts());
			clang::Rewriter::RewriteOptions opts;
			opts.RemoveLineIfEmpty = true; // optie heeft een FIXME in de docs, bug reports tonen dat deze plaatsvindt bij meerdere edits rond dezelfde regel.
			R->RemoveText(SourceRange(range.getBegin(), afterSemi),opts);
		} 
	}
	
	static void removeDependencyFiles(struct config& cfg){
		llvm::outs() << "Removing definition header and source file. \n";
		fs::remove((cfg.inputDir / (cfg.markerDefinitionFileName + ".h")).string());  
		fs::remove((cfg.inputDir / (cfg.markerDefinitionFileName + ".c")).string());  
	}
	
	// Declarations
	virtual void processDecl(Decl* D) {}
	virtual void processFunctionDecl(FunctionDecl* FD){
		if (FD->getNameAsString() != markerName) return;
		if (FD->getNumParams() < 1) return;
		deleteNode(FD);
	}
	
	// Statements
	virtual void processDeclStmt(DeclStmt* DS) {
		
	}
	virtual void processForStmt(ForStmt* FS) {}
	virtual void processWhileStmt(WhileStmt* WS){}
	virtual void processIfStmt(IfStmt* IS) {}
	virtual void processSwitchStmt(SwitchStmt* SS){}
	virtual void processCaseStmt(CaseStmt* CS){}
	virtual void processCallExpr(CallExpr* CE){
	  	const clang::FunctionDecl *callee = CE->getDirectCallee();
	   	if (!callee) return; // indirect call (function pointer, etc.)
		
	   	std::string funcName = callee->getNameAsString();
	   	if (funcName != markerName) return;
	   	if (CE->getNumArgs() < 1) return;		
		deleteNode(CE);
	}
};
