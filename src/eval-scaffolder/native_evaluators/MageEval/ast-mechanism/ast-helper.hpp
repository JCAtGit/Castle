
#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include <iostream>

using namespace clang;
using namespace std;

std::optional<DynTypedNode> getPrevSibling(DynTypedNode node,  ASTContext &nodeAST){
	llvm::SmallVector<DynTypedNode, 16> indexedNodeSiblings;
	// Safe to assume single parent for marker.
	DynTypedNodeList nodeParents = nodeAST.getParents(node);
	
	for (const auto &nodeParent : nodeParents) {
		if(const Stmt *nodeParentStmt = nodeParent.get<Stmt>()){ // probably compound stmt
			for(auto child:nodeParentStmt->children()){
				indexedNodeSiblings.push_back(DynTypedNode::create(*child));
			}
			// Find index of Node and get next sibling
			for (size_t i = 0; i < indexedNodeSiblings.size(); ++i) {
			    if (indexedNodeSiblings[i] == node && i - 1 > 0) {
					return indexedNodeSiblings[i - 1];
				}
			}
		} else {
			// TODO
		}
	}
	return std::nullopt;
}

std::optional<DynTypedNode> getNextSibling(DynTypedNode node, ASTContext &nodeAST) {
    llvm::SmallVector<DynTypedNode, 16> indexedNodeSiblings;
    auto nodeParents = nodeAST.getParents(node);
    if (nodeParents.empty()) {
        cout << "No parents found!" << endl;
        return nullopt;
    }
    for (const auto &nodeParent : nodeParents) {
        if (const Stmt *parentStmt = nodeParent.get<Stmt>()) {
            for (auto *child : parentStmt->children()) {
                indexedNodeSiblings.push_back(DynTypedNode::create(*child));
            }
            for (size_t i = 0; i < indexedNodeSiblings.size(); ++i) {
                if (indexedNodeSiblings[i] == node) {
                    if (i + 1 < indexedNodeSiblings.size()) {
                        return indexedNodeSiblings[i + 1];   // fixed
                    } else {
                        cout << "Node is last child, no next sibling" << endl;
                        return nullopt;
                    }
                }
            }
            cout << "Node not found among children of this parent" << endl;
        } else {
            cout << "Parent is not a Stmt, cannot get siblings" << endl;
        }
    }
    return nullopt;
}
//TODO
std::optional<DynTypedNode> getPrevSiblingMarker(DynTypedNode node, ASTContext &nodeAST){
	llvm::SmallVector<DynTypedNode, 16> indexedNodeSiblings;
				// Safe to assume single parent for marker.
				DynTypedNodeList nodeParents = nodeAST.getParents(node);
				for (const auto &nodeParent : nodeParents) {
					if(const Stmt *nodeParentStmt = nodeParent.get<Stmt>()){ // probably compound stmt
						for(auto child:nodeParentStmt->children()){
							indexedNodeSiblings.push_back(DynTypedNode::create(*child));
						}
						// Find index of Node and get next sibling
						for (size_t i = 0; i < indexedNodeSiblings.size(); ++i) {
						    if (indexedNodeSiblings[i] == node && i + 1 < indexedNodeSiblings.size()) {
								return indexedNodeSiblings[i - 1];
							}
						}
					} else {
						// TODO
					}
				}
				return nullopt;
		}

//TODO
std::optional<DynTypedNode> getNextSiblingMarker(DynTypedNode node, ASTContext &nodeAST){
	llvm::SmallVector<DynTypedNode, 16> indexedNodeSiblings;
	// Safe to assume single parent for marker.
	DynTypedNodeList nodeParents = nodeAST.getParents(node);
	for (const auto &nodeParent : nodeParents) {
		if(const Stmt *nodeParentStmt = nodeParent.get<Stmt>()){ // probably compound stmt
			for(auto child:nodeParentStmt->children()){
				indexedNodeSiblings.push_back(DynTypedNode::create(*child));
			}
			// Find index of Node and get next sibling
			for (size_t i = 0; i < indexedNodeSiblings.size(); ++i) {
			    if (indexedNodeSiblings[i] == node && i + 1 < indexedNodeSiblings.size()) {
					return indexedNodeSiblings[i];
				}
			}
		} else {
			// TODO
		}
	}
	return nullopt;
}

std::optional<DynTypedNode> getNextCallExprSameFunction(DynTypedNode node, ASTContext &nodeAST) {
    // 1. Verify the input is a CallExpr
    const CallExpr *originalCall = node.get<CallExpr>();
    if (!originalCall) {
        return nullopt;   // Input is not a call
    }

    // 2. Get the original callee's FunctionDecl
    const FunctionDecl *originalCallee = originalCall->getDirectCallee();
    if (!originalCallee) {return nullopt;}   // Indirect call – cannot compare

    // 3. Find the parent that can have multiple children (e.g., CompoundStmt)
    DynTypedNodeList parents = nodeAST.getParents(node);
    for (const DynTypedNode &parent : parents) {
        const Stmt *parentStmt = parent.get<Stmt>();
        if (!parentStmt) continue;

        // Collect all direct children of this parent
        llvm::SmallVector<DynTypedNode, 16> children;
        for (const Stmt *child : parentStmt->children()) {
            children.push_back(DynTypedNode::create(*child));
        }

        // Find the index of the original node
        size_t idx = 0;
        for (; idx < children.size(); ++idx) {
            if (children[idx] == node) break;
        }
        if (idx == children.size()) continue; // Not found? should not happen

        // 4. Scan forward from the next child
        for (size_t i = idx + 1; i < children.size(); ++i) {
            const Stmt *candidateStmt = children[i].get<Stmt>();
            if (!candidateStmt) continue;

            // In Clang, a CallExpr can appear directly as a child of a CompoundStmt.
            const CallExpr *candidateCall = dyn_cast<CallExpr>(candidateStmt);
            if (!candidateCall) continue;

            // 5. Compare the callee
            const FunctionDecl *candidateCallee = candidateCall->getDirectCallee();
            if (candidateCallee && candidateCallee == originalCallee) {
                return DynTypedNode::create(*candidateCall);
            }
        }
    }

    return nullopt;
}