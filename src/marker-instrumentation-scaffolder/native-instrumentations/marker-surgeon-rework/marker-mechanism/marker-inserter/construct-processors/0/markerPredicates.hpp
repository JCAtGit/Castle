#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"

using namespace std;
using namespace clang;

	const Stmt *getPreviousSibling(const Stmt *S, ASTContext &Ctx) {
		if (!S) return nullptr;
	    auto Parents = Ctx.getParents(*S);
	    if (Parents.empty()) return nullptr;
	    const CompoundStmt *Parent = Parents[0].get<CompoundStmt>();
	    if (!Parent) return nullptr;
	
	    // Use 'auto' to deduce the correct type: ArrayRef<Stmt*>
	    auto Children = Parent->body();
	    auto It = llvm::find(Children, S);
	    if (It == Children.end() || It == Children.begin()) return nullptr;
	    return *(It - 1);
	}
	
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
	
// Implement and replace as extraction for statements
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
		
		//TODO Check parents until root for recoveryExpr type and ...
		return true;
	}
	
	// Check DeclRefExpr and recoveryExpr
	bool stmtHasValidChildCriteria(Stmt* S){
		auto children = S->children();
		
		for(auto child:children){
			if (child == nullptr) {
//				llvm::errs() << "Null child in " << S->getStmtClassName() << "\n";
			    continue;
			}
//			if(isa<ImplicitCastExpr>(child)){return false;}
			if(isa<RecoveryExpr>(child)){return false;}
//			if(isa<DeclRefExpr>(child)){return false;}
		}
		return true;
	}
	
	bool stmtHasValidPrevSibling(Stmt* S, ASTContext &context){
		const Stmt *prev = getPreviousSibling(S, context);
		auto children = S->children();
		if(prev == nullptr){return true;}
		if(isa<ImplicitCastExpr>(prev)){
			return false;
		}
		
		for(auto *child:children){
			if(isa<ImplicitCastExpr>(child)){return false;}
		}
		
		return true;
	}
	
	bool stmtHasValidNextSibling(){
		return false;
	}
	
	bool stmtHasValidSurroundings(){
		return false;
	}
	
	bool hasUnsafeAncestor(Stmt *S, ASTContext &Context) {
	    llvm::SmallVector<DynTypedNode, 8> Stack;
	    Stack.push_back(DynTypedNode::create(*S));
	    while (!Stack.empty()) {
	        DynTypedNode N = Stack.pop_back_val();
	        // Check this node
	        if (const Stmt *St = N.get<Stmt>()) {
	            if (isa<RecoveryExpr>(St) ||
	                isa<DependentScopeDeclRefExpr>(St) ||
	                isa<UnresolvedLookupExpr>(St) ||
	                isa<CXXDependentScopeMemberExpr>(St)) {
	                return true;
	            }
	            if (St->getBeginLoc().isMacroID()) return true;
	        }
	        // Add parents to stack
	        for (const auto &Parent : Context.getParents(N)) {
	            Stack.push_back(Parent);
	        }
	    }
	    return false;
	}
	
	