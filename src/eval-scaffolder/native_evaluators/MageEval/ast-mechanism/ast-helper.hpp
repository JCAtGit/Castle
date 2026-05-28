
#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include <nlohmann/json.hpp>

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
    for (const Stmt *child : parent->children()) {
        children.push_back(DynTypedNode::create(*child));
    }
    return children;
}

optional<DynTypedNode> getNextSiblingFromList(
    const llvm::SmallVectorImpl<DynTypedNode> &siblings, size_t idx) {
    if (idx + 1 < siblings.size())
        return siblings[idx + 1];
    return std::nullopt;
}

optional<DynTypedNode> getPrevSiblingFromList(
    const llvm::SmallVectorImpl<DynTypedNode> &siblings, size_t idx) {
    if (idx > 0)
        return siblings[idx - 1];
    return std::nullopt;
}

// Search forward from index i for a CallExpr calling the same function
optional<DynTypedNode> findNextSiblingCall(const llvm::SmallVectorImpl<DynTypedNode> &children,
                                          size_t startIdx,
                                          const FunctionDecl *targetCallee) {
    for (size_t i = startIdx + 1; i < children.size(); ++i) {
        if (const CallExpr *candidate = children[i].get<CallExpr>()) {
            if (const FunctionDecl *callee = candidate->getDirectCallee()) {
                if (callee == targetCallee)
                    return children[i];
            }
        }
    }
    return std::nullopt;
}

// Similarly for previous
optional<DynTypedNode> findPrevSiblingCall(const llvm::SmallVectorImpl<DynTypedNode> &children,
                                          size_t startIdx,
                                          const FunctionDecl *targetCallee) {
    for (size_t i = startIdx; i > 0; --i) {
        size_t prevIdx = i - 1;
        if (const CallExpr *candidate = children[prevIdx].get<CallExpr>()) {
            if (const FunctionDecl *callee = candidate->getDirectCallee()) {
                if (callee == targetCallee)
                    return children[prevIdx];
            }
        }
    }
    return std::nullopt;
}

void recordTypeInfo(const QualType& T, json& storage){
	json paramTypeInfo;
	paramTypeInfo["type"] = T.getAsString();
	paramTypeInfo["is_const"] = T.isConstQualified();
	paramTypeInfo["is_pointer"] = T->isPointerType();
	storage.push_back(
		paramTypeInfo
	);	
}

