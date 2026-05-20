#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <optional>
#include <unordered_map>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTTypeTraits.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTStructuralEquivalence.h"

#include "llvm/Support/raw_os_ostream.h"
#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class CrossASTVisitor: public RecursiveASTVisitor<CrossASTVisitor> {
	private:
	json& markerFormat;
	ASTContext &oracleAST, &decomAST;
	SourceManager &oracleSM, &decomSM;
		
	unordered_map<int64_t, CallExpr*> decomMarkerMap;
	unordered_map<int64_t, CallExpr*> oracleMarkerMap;
	
	unordered_map<int64_t, CompoundStmt*> decomMarkerParentMap;
	unordered_map<int64_t, CompoundStmt*> oracleMarkerParentMap;

	
	int proc_mode = 0; // 0 = oracle, 1 = decom
	void (CrossASTVisitor::*selectedProcessor)(Stmt *S);
	ASTContext* selectedAST;
	SourceManager* selectedSM;
	string selectedMessage;
	unordered_map<int64_t, CallExpr*>* 		selectedMarkerMap;
	unordered_map<int64_t, CompoundStmt*>* 	selectedMarkerParentMap;

	StructuralEquivalenceContext::NonEquivalentDeclSet NonEquivalentDecls;
	StructuralEquivalenceContext seContext;

	public:	
	CrossASTVisitor(json& markerFormat, ASTContext &oracleAST, ASTContext &decomAST): 
	markerFormat(markerFormat),	oracleAST(oracleAST), decomAST(decomAST),
	
	oracleSM(oracleAST.getSourceManager()), decomSM(decomAST.getSourceManager()), 
	seContext(
		StructuralEquivalenceContext(
			oracleAST.getLangOpts(),
		    oracleAST,
		    decomAST,
		    NonEquivalentDecls,
		    StructuralEquivalenceKind::Default,
		    false, 						/*StrictTypeSpelling=*/
		    true, 								// Complain, structural errors emit straight to your TextDiagnosticPrinter
		    true 					/*ErrorOnTagTypeMismatch=*/
		)
	)
	{}
	
	void selectMode(int proc_mode){
		switch (proc_mode) {
			case 0:
			selectedProcessor = &CrossASTVisitor::indexProcessor;
			selectedAST = &decomAST;
			selectedSM = &decomSM;
			selectedMarkerMap = &decomMarkerMap;
			selectedMarkerParentMap = &decomMarkerParentMap;
			selectedMessage = "decomIndexProcessor";
			break;
			case 1:
			selectedProcessor = &CrossASTVisitor::indexProcessor;
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
			
			// Build parents cache for markerCE.
			auto markerCEParents = selectedAST->getParents(*markerCE);
			auto markerCEParent = markerCEParents[0];
			auto markerCEParentStmt = markerCEParent.get<CompoundStmt>();
			(*selectedMarkerParentMap)[intValue] = const_cast<CompoundStmt*>(markerCEParentStmt);
			
			cout << DC->getNameInfo().getName().getAsString() << " : " << intValue << " : " << markerCEParents.size() << endl; 
//			if(!markerCEParentStmt){return;}
//			auto siblings = getChildrenNodes(markerCEParentStmt);
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
			auto MCEDecomParent = decomMarkerParentMap[0];
			auto MCEDecomSiblings = getChildrenNodes(MCEDecomParent);
			auto MCEDecomSibIndex = findNodeSiblingIndex(MCEDecomSiblings, DynTypedNode::create(*MCEDecom));
			auto MCEDecomMarkee = getNextSiblingFromList(MCEDecomSiblings, MCEDecomSibIndex);
			
//			if(!MCEDecomMarkee){
//				cout << "MCEDecomMarkee is NULL" << endl;
//			} else {
//				cout << "MCEDecomMarkee is not NULL" << endl;
//			}
//			
			if(MCEOracleMarkee->get<Stmt>() && MCEDecomMarkee->get<Stmt>()){
				Stmt* s1 = const_cast<Stmt*>(MCEOracleMarkee->get<Stmt>());
				Stmt* s2 = const_cast<Stmt*>(MCEDecomMarkee->get<Stmt>());
				auto result = seContext.IsEquivalent(s1, s2);
				cout << "== result:" << result << endl;
			} else if(MCEOracleMarkee->get<Decl>() && MCEDecomMarkee->get<Decl>()){
				cout << "== result:decl" << endl;
			} else {
				cout << "== result:else" << endl;
				MCEOracleMarkee->dump(llvm::outs(), oracleAST);
				cout << "-	-	-	-	-" << endl;
				MCEDecomMarkee->dump(llvm::outs(), decomAST);
			}
		}
	}
	
	// Evaluates by traversing through the oracle and then mapping onto the target.
	bool VisitStmt(Stmt *S) {
		(this->*selectedProcessor)(S);
		return true;
	}
};

