
#pragma once

#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include "clang/Analysis/CFG.h"
#include "llvm/Support/Casting.h"
#include <cstddef>
#include <iostream>
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
    for (auto child : parent->children()) {
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

// TODO:
//bool hasFallthroughDepth(Stmt * S, bool& conditional, json& switchCases, int depth = -1){
//	vector<bool> branches; // ifs and whatnot
//	if (!S) return true; // empty statement -> fallthrough
//	depth++;
//	switch (S->getStmtClass()) {
//		case Stmt::DefaultStmtClass:{
//			if(depth>0){
//				json caseInfo = json::object();	
//				caseInfo["has_fallthrough"] = true;
//				caseInfo["condition_fallthrough"] = conditional;
//				caseInfo["case_stmt"] = S->getStmtClassName();
//				caseInfo["case_sub_stmt"] = llvm::dyn_cast<DefaultStmt>(S)->getSubStmt()->getStmtClassName();
//				switchCases.push_back(caseInfo);
//				conditional = false;
//				return true;
//			}
//		}
//			
//		case Stmt::CaseStmtClass:{
//			if(depth>0){
//				json caseInfo = json::object();	
//				caseInfo["has_fallthrough"] = true;
//				caseInfo["condition_fallthrough"] = conditional;
//				caseInfo["case_stmt"] = S->getStmtClassName();
//				caseInfo["case_sub_stmt"] = llvm::dyn_cast<DefaultStmt>(S)->getSubStmt()->getStmtClassName();
//				switchCases.push_back(caseInfo);
//				conditional = false;
//				return true;
//			}
//		}
//		default: 
//			if (auto *E = llvm::dyn_cast<Expr>(S)) {
//				return true;
//			}
//			for (auto child : S->children()){
//				branches.push_back(hasFallthroughDepth(child, conditional, switchCases, depth));
//			}
//			for(int i=1;i<branches.max_size();i++){
//				if(branches[0] != branches[i]){
//					return true;
//				}
//			}
//			if(branches.empty()){
//				return true;
//			} else {
//				return branches[0];
//			}
////		case Stmt::CompoundStmtClass:break;
//		case Stmt::IfStmtClass:	{
//			for (auto child : S->children()){
//				branches.push_back(hasFallthroughDepth(child, conditional, switchCases, depth));
//			}
//			for(int i=1;i<branches.max_size();i++){
//				if(branches[0] != branches[i]){
//					conditional = true;
//					return true;
//				}
//			}
//			if(branches.empty()){
//				return true;
//			} else {
//				return branches[0];
//			}
//		}
//		case Stmt::BreakStmtClass: 		return false;
//        case Stmt::ReturnStmtClass: 	return false;
//        case Stmt::GotoStmtClass: 		return false;
//        case Stmt::ContinueStmtClass:	return false;
//		case Stmt::SwitchStmtClass:		return true;
//     	case Stmt::WhileStmtClass:		return true;
//	    case Stmt::DoStmtClass:			return true;
//	    case Stmt::ForStmtClass:		return true;	
//	}
//}

//void hasFallthroughWidth(SwitchStmt* SS, json& switchCases){
//	auto switchChildren = SS->getBody()->children();
////	SS->getSwitchCaseList()->
//    SwitchCase* currentCase = nullptr;
//	vector<std::pair<bool, bool>> child_results;
//	bool conditional = false;
//	int index = 1;
//	for(auto child: switchChildren){
//		cout << "child: " << child->getStmtClassName() << "\n";
//		if(llvm::dyn_cast<SwitchCase>(child)){
//			cout << "cast case: " << child->getStmtClassName() << "\n";
//			if(currentCase == nullptr){
//				cout << "Assign currentCase: " << child->getStmtClassName() << "\n";
//				currentCase = llvm::dyn_cast<SwitchCase>(child);
//			} else {
//				cout << "Tally currentCase: " << child->getStmtClassName() << "\n";
//				for(;index<child_results.max_size();index++){
//					if(child_results[0] != child_results[index]){
//						break;
//					}
//				}
//				cout << "defaultChild: " << child_results[0].first << "\n";
//				json caseInfo;
//				if(child_results.size() <= 1){
//					cout << "noChild" << "\n";
//					caseInfo["has_fallthrough"] = child_results[index].first;
//					caseInfo["condition_fallthrough"] = conditional;
//					caseInfo["case_stmt"] = currentCase->getStmtClassName();
//					caseInfo["case_sub_stmt"] = currentCase->getSubStmt()->getStmtClassName();
//				} else {
//					cout << "yesChild: " << child_results.size() << "\n";
//					cout << "indexChild: " << child_results[index].first << "\n";
//
//					caseInfo["has_fallthrough"] = child_results[index].first;
//					caseInfo["condition_fallthrough"] = conditional;
//					caseInfo["case_stmt"] = currentCase->getStmtClassName();
//					caseInfo["case_sub_stmt"] = currentCase->getSubStmt()->getStmtClassName();
//				}
//				cout << caseInfo.dump(2) << "\n";
//				
//				switchCases.push_back(caseInfo);
//				cout << "Assign new case" << "\n";
//				currentCase = llvm::dyn_cast<SwitchCase>(child);
//				conditional = false;	
//				index = 1;
//				child_results.clear();
//			}
//		}
//		if(!currentCase) continue;
//		child_results.push_back({hasFallthroughDepth(child,conditional,switchCases), conditional});
//	}
//	if(currentCase != NULL && switchCases.empty()){
//		json caseInfo;
//		caseInfo["has_fallthrough"] = child_results[0].first;
//		caseInfo["condition_fallthrough"] = child_results[0].second;
//		caseInfo["case_stmt"] = currentCase->getStmtClassName();
//		caseInfo["case_sub_stmt"] = currentCase->getSubStmt()->getStmtClassName();
//	}
//}


