#pragma once

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceLocation.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include "../../../../config/config_ds.hpp"
#include "../../../../../include/helper.hpp"
#include "../RemoverConstructProcessorInterface.hpp"

using namespace clang;
using namespace std;

class RCP1: public RemoverConstructProcessor {
	private:
		ASTContext* context;
		Rewriter* R; 	
		string markerName;
	public:
	RCP1(
		ASTContext* context, 
		Rewriter* R, 
		std::string markerName
	) : context(context), R(R), markerName(markerName){}

	const Stmt *getPrecedingSibling(const Stmt *Node, const CompoundStmt *parent) {
	    if (!parent) return nullptr;
	    
	    auto children = parent->body();  // returns a body_range
	    auto It = std::find(children.begin(), children.end(), Node);
	    if (It == children.begin() || It == children.end())
	        return nullptr;
	    
	    return *(It - 1);
	}
	
	const Stmt *getFollowingSibling(const Stmt *Node, const CompoundStmt *parent) {
	    if (!parent) return nullptr;
	    
	    const auto &children = parent->body();  // body_range
	    auto It = std::find(children.begin(), children.end(), Node);
	    if (It == children.end() || std::next(It) == children.end())
	        return nullptr;
	    
	    return *(std::next(It));
	}
	
	void deleteMarkerTriplet(Stmt* S){
		auto parents = context->getParents(*S);
		if (parents.empty()) return;   
	  	const auto *parent = parents[0].get<CompoundStmt>();
	   	if (!parent) return;
		auto precSibling = getPrecedingSibling(S, parent);
		auto follSibling = getFollowingSibling(S, parent);
		if(precSibling && follSibling){
			deleteNode(precSibling);
			deleteNode(S);
			deleteNode(follSibling);
		}
	}
	
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
		fs::remove((cfg.inputDir / (cfg.markerDefinitionFileName + ".h")).string());  
	}
	
	// Declarations
	virtual void processDecl(Decl* D) {}
	virtual void processFunctionDecl(FunctionDecl* FD){
		if (FD->getNameAsString() != markerName) return;
		if (FD->getNumParams() < 1) return;
		deleteNode(FD);
	}
	
	// Statements
	virtual void processDeclStmt(DeclStmt* DS) {}
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
		deleteMarkerTriplet(CE);
	}
};
