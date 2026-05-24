#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTStructuralEquivalence.h"

#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_os_ostream.h"
#include "llvm/Support/raw_ostream.h"
#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class CrossASTGatherer: public RecursiveASTVisitor<CrossASTGatherer> {
	private:
	json &markerFormat, &genEval, &specEval, &markerEval, &humanEval;
	ASTContext &oracleAST, &decomAST;
	SourceManager &oracleSM, &decomSM;
		
	unordered_map<int64_t, CallExpr*> decomMarkerMap;
	unordered_map<int64_t, CallExpr*> oracleMarkerMap;
	
	unordered_map<int64_t, CompoundStmt*> decomMarkerParentMap;
	unordered_map<int64_t, CompoundStmt*> oracleMarkerParentMap;

	int proc_mode = 0; // 0 = oracle, 1 = decom
	void (CrossASTGatherer::*selectedProcessor)(Stmt *S);
	ASTContext* selectedAST;
	SourceManager* selectedSM;
	string selectedMessage;
	unordered_map<int64_t, CallExpr*>* 		selectedMarkerMap;
	unordered_map<int64_t, CompoundStmt*>* 	selectedMarkerParentMap;
	unordered_map<CallExpr*, CallExpr*> 	markerParentMarkerMap;
	
	StructuralEquivalenceContext::NonEquivalentDeclSet neds;
	StructuralEquivalenceContext sec;

	public:	
	CrossASTGatherer(
		json& markerFormat, json&genEval, json& specEval, json& markerEval, json& humanEval,
		ASTContext &oracleAST, ASTContext &decomAST): 
	markerFormat(markerFormat),	genEval(genEval), specEval(specEval), markerEval(markerEval), humanEval(humanEval),
	oracleAST(oracleAST), decomAST(decomAST),
	
	oracleSM(oracleAST.getSourceManager()), decomSM(decomAST.getSourceManager()), 
	sec(
		StructuralEquivalenceContext(
			oracleAST.getLangOpts(),
		    oracleAST,
		    decomAST,
		    neds,
		    StructuralEquivalenceKind::Default,
		    false, 						/*StrictTypeSpelling=*/
		    true, 								// Complain, structural errors emit straight to your TextDiagnosticPrinter
		    true 					/*ErrorOnTagTypeMismatch=*/
		)
	){}
	
	void selectMode(int proc_mode){
		switch (proc_mode) {
			case 0:
			selectedProcessor = &CrossASTGatherer::indexProcessor;
			selectedAST = &decomAST;
			selectedSM = &decomSM;
			selectedMarkerMap = &decomMarkerMap;
			selectedMarkerParentMap = &decomMarkerParentMap;
			selectedMessage = "decomIndexProcessor";
			break;
			case 1:
			selectedProcessor = &CrossASTGatherer::indexProcessor;
			selectedAST = &oracleAST;
			selectedSM = &oracleSM;
			selectedMarkerMap = &oracleMarkerMap;
			selectedMarkerParentMap = &oracleMarkerParentMap;
			selectedMessage = "oracleIndexProcessor";
			break;			
			default:
			selectedProcessor = NULL;
			selectedMessage = "No processor selected";
			break;
		}
		cout << selectedMessage << endl;
	}
	
	void indexProcessor(Stmt *S){
		if (selectedSM->isInMainFile(S->getBeginLoc()) && S->getStmtClass() == Stmt::CallExprClass){
			CallExpr* markerCE = cast<CallExpr>(S);
			auto DC = markerCE->getDirectCallee();
			if(DC == nullptr){return;}
			if(DC->getNameInfo().getName().getAsString() != markerFormat["name"].get<string>()){return;} // TODO?: check comment at markerformat declaration.
			
			auto *IL = dyn_cast<IntegerLiteral>(markerCE->getArg(0)->IgnoreImpCasts());
			if (!IL) {return;}
			llvm::APInt Value = IL->getValue();
		    int64_t intValue = Value.getSExtValue();
			(*selectedMarkerMap)[intValue] = markerCE;
			
			// Build parent map for markerCE.
			auto markerCEParents = selectedAST->getParents(*markerCE);
			auto markerCEParent = markerCEParents[0];
			auto markerCEParentStmt = markerCEParent.get<CompoundStmt>();
			(*selectedMarkerParentMap)[intValue] = const_cast<CompoundStmt*>(markerCEParentStmt);
			
//			cout << DC->getNameInfo().getName().getAsString() << " : " << intValue << " : " << markerCEParents.size() << endl; 
			
//			// Get parent marker of markerCE.
//			if(!markerCEParentStmt){return;}
//				auto siblings = getChildrenNodes(markerCEParentStmt);
//			int markerIndex = findNodeSiblingIndex(siblings, DynTypedNode::create(*markerCE));
//			
//			// Build parents cache for markedNode.
//			auto markedNode = getNextSiblingFromList(siblings, markerIndex);
//			auto markedNodeParents = selectedAST->getParents(*markedNode);
		}
	}
	
	void evalProcessor(){
		cout << "OracleMap:" << oracleMarkerMap.size() << " - DecomMap:" << decomMarkerMap.size() << endl; 
		cout << "OracleParentMap:" << oracleMarkerParentMap.size() << " - DecomParentMap:" << decomMarkerParentMap.size() << endl; 

		for(auto& [markerID, markerCE]: oracleMarkerMap){
			if(decomMarkerMap.find(markerID) == decomMarkerMap.end()){
				cout << "ID:" << markerID << " missing" << endl; 
				continue; // maybe do something else too
			}	
			cout << "ID:" << markerID << " present" << endl; 
			auto MCEOracle = markerCE;
			auto MCEOracleParent = oracleMarkerParentMap.at(markerID);
			auto MCEOracleSiblings = getChildrenNodes(MCEOracleParent);
			auto MCEOracleSibIndex = findNodeSiblingIndex(MCEOracleSiblings, DynTypedNode::create(*MCEOracle));
			auto MCEOracleMarkee = getNextSiblingFromList(MCEOracleSiblings, MCEOracleSibIndex);

			auto MCEDecom = decomMarkerMap.at(markerID);
			auto MCEDecomParent = decomMarkerParentMap.at(markerID);
			auto MCEDecomSiblings = getChildrenNodes(MCEDecomParent);
			auto MCEDecomSibIndex = findNodeSiblingIndex(MCEDecomSiblings, DynTypedNode::create(*MCEDecom));
			auto MCEDecomMarkee = getNextSiblingFromList(MCEDecomSiblings, MCEDecomSibIndex);
			
//			if(!MCEDecomMarkee){cout << "MCEDecomMarkee is NULL" << endl;} 
//			else {cout << "MCEDecomMarkee is not NULL" << endl;}
	
			if(MCEOracleMarkee->get<Stmt>() && MCEDecomMarkee->get<Stmt>()){
				Stmt* s1 = const_cast<Stmt*>(MCEOracleMarkee->get<Stmt>());
				Stmt* s2 = const_cast<Stmt*>(MCEDecomMarkee->get<Stmt>());
				auto result = sec.IsEquivalent(s1, s2);
				llvm::outs() << "== result stmt:" << result << "|" << MCEOracleMarkee->getNodeKind() << " - " << MCEDecomMarkee->getNodeKind() <<  '\n';
//				MCEOracleParent->dump(llvm::outs(), oracleAST);
//				cout << endl;
//				MCEDecomParent->dump(llvm::outs(), decomAST);
			} else if(MCEOracleMarkee->get<Decl>() && MCEDecomMarkee->get<Decl>()){
				cout << "== result decl:" << endl;
				Decl* d1 = const_cast<Decl*>(MCEOracleMarkee->get<Decl>());
				Decl* d2 = const_cast<Decl*>(MCEDecomMarkee->get<Decl>());
				auto result = sec.IsEquivalent(d1, d2);
				cout << "== result:" << result << endl;
//				MCEOracleParent->dump(llvm::outs(), oracleAST);
//				cout << endl;
//				MCEDecomParent->dump(llvm::outs(), decomAST);
			} else {
				cout << "== result else:" << endl;
//				MCEOracleParent->dump(llvm::outs(), oracleAST);
//				cout << endl;
//				MCEDecomParent->dump(llvm::outs(), decomAST);
			}
			
			string makerIDString = std::to_string(markerID);
			genEval[makerIDString]["oracle"]["ASTNode"] = MCEOracleMarkee->getNodeKind().asStringRef();
			genEval[makerIDString]["oracle"]["children"] = json::array();
			genEval[makerIDString]["oracle"]["properties"] = json::object();;

			genEval[makerIDString]["decom"]["ASTNode"] = MCEDecomMarkee->getNodeKind().asStringRef();
			genEval[makerIDString]["decom"]["children"] = json::array();
			genEval[makerIDString]["decom"]["properties"] = json::object();;
			
			evalSRDecider(const_cast<Stmt*>(MCEOracleMarkee->get<Stmt>()), &(genEval[makerIDString]["oracle"][MCEOracleMarkee->getNodeKind().asStringRef()]));
			evalSRDecider(const_cast<Stmt*>(MCEDecomMarkee->get<Stmt>()), &(genEval[makerIDString]["decom"][MCEDecomMarkee->getNodeKind().asStringRef()]));
		}
	}
	
	// Evaluates by traversing through the oracle and then mapping onto the target.
	bool VisitStmt(Stmt *S) {(this->*selectedProcessor)(S);return true;}
		
	const std::unordered_set<Stmt::StmtClass> traversalSkipList = {
		Stmt::IfStmtClass,
		Stmt::WhileStmtClass,
		Stmt::ForStmtClass,
		Stmt::SwitchStmtClass,
//		Stmt::CallExprClass,
		Stmt::CompoundStmtClass
	};
	
	void evalSRDecider(Stmt *node, json *findings){
		switch(node->getStmtClass()){
			default: generalStmtRecorder(node, findings); break;
			case Stmt::DeclStmtClass: declStmtRecorder(node, findings); break;
		}
	}

	void generalStmtRecorder(Stmt *node, json *findings){
		auto stmtChildren = node->children();
		for(auto childNode:stmtChildren){
			if (!childNode) {continue;}
			if(traversalSkipList.find(childNode->getStmtClass()) != traversalSkipList.end()){continue;}
			string nodeClassString = childNode->getStmtClassName();
			json child;
			child["ASTNode"] = nodeClassString;
			child["children"] = json::array();
			child["properties"] = json::object();
			(*findings)["children"].push_back(child);
			json &storedChild = (*findings)["children"].back();
			evalSRDecider(childNode, &storedChild);
		}
	}
	
	void declStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<clang::DeclStmt>(node);
		auto declChildren = cast_node->decls();
		for(auto declChild : declChildren){
			if(!declChild){continue;}
			string nodeClassString = declChild->getDeclKindName();
			json child = {};
			child["ASTNode"] = nodeClassString;
			child["children"] = json::array();
			child["properties"] = json::object();
			(*findings)["children"].push_back(child);
			json &storedChild = (*findings)["children"].back();
			evalDRDecider(declChild, &storedChild);
		}
	}


	void evalDRDecider(Decl *node, json *findings){
		switch(node->getKind()){
			default: generalDeclRecorder(node, findings); break;
			case Decl::Var: declVarRecorder(node, findings); break;
		}
	}
	
	// entry should give diagnostic effects
	void generalDeclRecorder(Decl *node, json *findings){}
	
	void declVarRecorder(Decl *node, json *findings){	
		auto cast_node = llvm::cast<VarDecl>(node);
		auto cn_init = cast_node->getInit();
		if(cn_init){
			string nodeClassString = cn_init->getType().getAsString();
			json child = {};
			child["ASTNode"] = nodeClassString;
			child["children"] = json::array();
			child["properties"] = json::object();
			(*findings)["children"].push_back(child);
			json &storedChild = (*findings)["children"].back();
		}
	}
};

