
#pragma once

#include "clang/AST/ParentMapContext.h"
#include "nlohmann/json.hpp"
#include "clang/AST/ExprCXX.h"

using namespace clang;
using namespace std;
using namespace nlohmann;

size_t findNodeSiblingIndex(const llvm::SmallVectorImpl<DynTypedNode> &siblings, const DynTypedNode &node) {
    for (size_t i = 0; i < siblings.size(); ++i) {
        if (siblings[i] == node) return i;
    }
    return siblings.size(); // not found
}

llvm::SmallVector<DynTypedNode, 16> getChildrenNodes(const Stmt *parent) {
    llvm::SmallVector<DynTypedNode, 16> children;
    for (auto child : parent->children()) {
		if(child){
			children.push_back(DynTypedNode::create(*child));
		}
    }
    return children;
}

optional<DynTypedNode> getNextSiblingFromList(
    const llvm::SmallVectorImpl<DynTypedNode> &siblings, 
	size_t idx
) {
    if (idx + 1 < siblings.size())
        return siblings[idx + 1];
    return std::nullopt;
}

optional<DynTypedNode> getPrevSiblingFromList(
    const llvm::SmallVectorImpl<DynTypedNode> &siblings, 
	size_t idx) {
    if (idx > 0)
        return siblings[idx - 1];
    return std::nullopt;
}

//void recordCTypeInfo(const Type& T, json& storage){
//	json paramTypeInfo;
//	paramTypeInfo["type"] = T.getCanonicalType().getAsString();
//	paramTypeInfo["is_const"] = T.isConstQualified();
//	paramTypeInfo["is_pointer"] = T->isPointerType();
//	storage.push_back(
//		paramTypeInfo
//	);	
//}

void recordQTypeInfo(const QualType& T, json& storage){
	json paramTypeInfo;
	// Calling getAsString will also show dev defined types.
	auto CT = T.getCanonicalType();
//	paramTypeInfo["type_class"] = CT->getTypeClassName();
	paramTypeInfo["type"] = CT.getAsString();
	paramTypeInfo["is_const"] = CT.isConstQualified();
	paramTypeInfo["is_pointer"] = CT->isPointerType();
	storage.push_back(
		paramTypeInfo
	);	
}

string getCallExprName(const CallExpr *CE, const ASTContext &Ctx) {
    if (!CE) return "";

    // Direct resolved callee
    if (const FunctionDecl *FD = CE->getDirectCallee()) {
        return FD->getDeclName().getAsString();
    }

    const Expr *Callee = CE->getCallee()->IgnoreParenImpCasts();

    // Dependent member access
    if (const CXXDependentScopeMemberExpr *DSME =
            dyn_cast<CXXDependentScopeMemberExpr>(Callee)) {
        return DSME->getMemberNameInfo().getAsString();
    }

    // Unresolved member access
    if (const UnresolvedMemberExpr *UME = dyn_cast<UnresolvedMemberExpr>(Callee)) {
        return UME->getMemberNameInfo().getAsString();
    }

    // Unqualified dependent call
    if (const UnresolvedLookupExpr *ULE = dyn_cast<UnresolvedLookupExpr>(Callee)) {
        return ULE->getName().getAsString();
    }

    // Qualified dependent call
    if (const DependentScopeDeclRefExpr *DSDRE =
            dyn_cast<DependentScopeDeclRefExpr>(Callee)) {
        return DSDRE->getDeclName().getAsString();
    }

    // Resolved DeclRefExpr (function pointer, etc.)
    if (const DeclRefExpr *DRE = dyn_cast<DeclRefExpr>(Callee)) {
        return DRE->getDecl()->getDeclName().getAsString();
    }

    // Resolved MemberExpr
    if (const MemberExpr *ME = dyn_cast<MemberExpr>(Callee)) {
        return ME->getMemberDecl()->getDeclName().getAsString();
    }

    // Overloaded operator
    if (const CXXOperatorCallExpr *OCE = dyn_cast<CXXOperatorCallExpr>(CE)) {
        return getOperatorSpelling(OCE->getOperator());
    }

    // Fallback
//	SourceRange Range = CE->getSourceRange();
//	StringRef Text = Lexer::getSourceText(CharSourceRange::getTokenRange(Range), Ctx.getSourceManager(), Ctx.getLangOpts());
    return "unknown name";
}

bool usesEqualsInitializer(const DeclStmt *DS) {
    for (const Decl *D : DS->decls()) {
        if (const VarDecl *VD = dyn_cast<VarDecl>(D)) {
            // Check if it has an initializer and that initializer uses '='.
            if (VD->hasInit() && VD->getInitStyle() == VarDecl::CInit) {
                return true;
            }
        }
    }
    return false;
}