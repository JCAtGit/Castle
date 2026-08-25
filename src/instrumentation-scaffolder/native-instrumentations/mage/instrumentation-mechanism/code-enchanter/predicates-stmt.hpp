#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include <iostream>

using namespace std;
using namespace clang;

	/**
	 *	Get previous sibling AST node, if any, within the Clang AST
	 *	@param S	Current statement node to position against
	 *	@param Ctx 	ASTContext of statement node
	 */
	const Stmt *getPreviousSibling(const Stmt *S, ASTContext &Ctx) {
		if (!S) return nullptr;
	    auto Parents = Ctx.getParents(*S);
	    if (Parents.empty()) return nullptr;
	    const CompoundStmt *Parent = Parents[0].get<CompoundStmt>();
	    if (!Parent) return nullptr;
	
	    auto Children = Parent->body();
	    auto It = llvm::find(Children, S);
	    if (It == Children.end() || It == Children.begin()) return nullptr;
	    return *(It - 1);
	}
	
	/**
	 *	Get next sibling AST node, if any, within the Clang AST
	 *	@param S	Current statement node to position against
	 *	@param Ctx 	ASTContext of statement node
	 */
	const Stmt *getNextSibling(const Stmt *S, ASTContext &Ctx) {
	    auto Parents = Ctx.getParents(*S);
	    if (Parents.empty()) return nullptr;
	    const CompoundStmt *Parent = Parents[0].get<CompoundStmt>();
	    if (!Parent) return nullptr;
	
	    auto Children = Parent->body();
	    auto It = llvm::find(Children, S);
	    if (It == Children.end() || It + 1 == Children.end()) return nullptr;
	    return *(It + 1);
	}
	
	/**
	 *	Checks if we mark statements that do not disrupt the syntax.
	 *	Sometimes DeclStmt and such can be present within condition clauses and such, which breaks upon marking.
	 */
	bool stmtHasValidParentCriteria(Stmt* S, ASTContext &context){
		DynTypedNodeList Parents = context.getParents(*S);
		for (const auto &Parent : Parents) {
			if (const auto *ParentStmt = Parent.get<Stmt>()) {
				if(ParentStmt->getBeginLoc().isMacroID()){return false;}
				// If parent node is one of the following, then we skip the marking.
				// Otherwise we mark the parameters within these constructs, leading to invalid syntax.
				if(isa<ForStmt>(ParentStmt)){return false;} 
				else if (isa<IfStmt>(ParentStmt)) {return false;} 
				else if (isa<SwitchStmt>(ParentStmt)) {return false;} 
				else if (isa<AttributedStmt>(ParentStmt)) {return false;}
			}
		}
		return true;
	}
	
	/**
	 *	TODO: Refactor: maybe existing macro check is enough, making this function redundant, but could be used to check for other cases.
	 *	Checks if child might contain nodes that could indicate macros or external unresolved dependencies that mess with marking.
	 */
	bool stmtHasValidChildCriteria(Stmt* S){
		auto children = S->children();
		for(auto child:children){
			if (child == nullptr) { // Null child
			    continue;
			}
//			if(isa<ImplicitCastExpr>(child)){return false;}
			if(isa<RecoveryExpr>(child)){
				return false;
			}
//			if(isa<DeclRefExpr>(child)){return false;}
		}
		return true;
	}
	