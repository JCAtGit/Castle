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

#include "llvm/Support/raw_ostream.h"
#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class CrossASTVisitor: public RecursiveASTVisitor<CrossASTVisitor> {
	private:
	json& markerFormat;
	ASTContext &oracleAST, &decomAST;
	SourceManager &oracleSM, &decomSM;
	
	unordered_map<int64_t, CallExpr*> decomMap;
	unordered_map<int64_t, CallExpr*> oracleMap;
	
	int proc_mode = 0; // 0 = oracle, 1 = decom
	void (CrossASTVisitor::*selectedProcessor)(Stmt *S);
	ASTContext* selectedAST;
	SourceManager* selectedSM;
	string selectedMessage;
	unordered_map<int64_t, CallExpr*>* selectedASTMap;
	
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
		    /*StrictTypeSpelling=*/false,
		    /*Complain=*/true, // Structural errors emit straight to your TextDiagnosticPrinter
		    /*ErrorOnTagTypeMismatch=*/true
		)
	)
	{}
	
	void selectMode(int proc_mode){
		switch (proc_mode) {
			case 0:
			selectedProcessor = &CrossASTVisitor::indexProcessor;
			selectedAST = &decomAST;
			selectedSM = &decomSM;
			selectedASTMap = &decomMap;
			selectedMessage = "decomIndexProcessor";
			break;
			case 1:
			selectedProcessor = &CrossASTVisitor::indexProcessor;
			selectedAST = &oracleAST;
			selectedSM = &oracleSM;
			selectedASTMap = &oracleMap;
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
			
			(*selectedASTMap)[intValue] = markerCE;
			
			auto parents = selectedAST->getParents(*markerCE);
			auto parent = parents[0];
			auto parentStmt = parent.get<CompoundStmt>();
	
			cout << DC->getNameInfo().getName().getAsString() << " : " << intValue << " : " << parents.size() <<endl; 
//			if(!parentStmt){return;}
//			auto siblings = getChildrenNodes(parentStmt);
//			int markerIndex = findNodeSiblingIndex(siblings, DynTypedNode::create(*markerCE));
//			auto markedNode = getNextSiblingFromList(siblings, markerIndex);
		}
	}
	
	void evalProcessor(){
		cout << "OracleMap:" << oracleMap.size() << " - DecomMap:" << decomMap.size() << endl; 

		for(auto& [markerID, markerCE]:oracleMap){
			auto markerCEOracle = markerCE;
			if(decomMap.find(markerID) == decomMap.end()){
				cout << "ID:" << markerID << " missing" << endl; 
				continue; // maybe do something else too
			}			
			cout << "ID:" << markerID << " present" << endl; 
			auto markerCEDecom = decomMap[markerID];
			
			
		}
	}
	
	// Evaluates by traversing through the oracle and then mapping onto the target.
	bool VisitStmt(Stmt *S) {
		(this->*selectedProcessor)(S);
		return true;
	}
	
};

