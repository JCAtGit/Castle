
#pragma once

#include "clang/AST/ParentMapContext.h"
#include "nlohmann/json.hpp"

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

