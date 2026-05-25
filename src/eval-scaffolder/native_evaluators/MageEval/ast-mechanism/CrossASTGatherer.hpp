#pragma once

#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "clang/AST/ASTStructuralEquivalence.h"

#include "clang/Frontend/TextDiagnosticPrinter.h"
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
	
	// Structural equivalence functionality
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
		    false, 								/*StrictTypeSpelling=*/
		    true, 								// Complain, structural errors emit straight to your TextDiagnosticPrinter
		    true								/*ErrorOnTagTypeMismatch=*/
		)
	)
	{}
	
	void selectMode(int proc_mode){
		switch (proc_mode) {
			case 0:
			selectedAST = &decomAST;
			selectedSM = &decomSM;
			selectedMarkerMap = &decomMarkerMap;
			selectedMarkerParentMap = &decomMarkerParentMap;
			selectedMessage = "decomIndexProcessor";
			break;
			case 1:
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
		}
	}
	
	void gatherFindings(){
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
	
			if(MCEOracleMarkee->get<Stmt>() && MCEDecomMarkee->get<Stmt>()){
				Stmt* s1 = const_cast<Stmt*>(MCEOracleMarkee->get<Stmt>());
				Stmt* s2 = const_cast<Stmt*>(MCEDecomMarkee->get<Stmt>());
				auto result = sec.IsEquivalent(s1, s2);
				llvm::outs() << "== result stmt:" << result << "|" << MCEOracleMarkee->getNodeKind() << " - " << MCEDecomMarkee->getNodeKind() <<  '\n';
				cout << "Diagnostics: " << DiagnosticStream.str() << "\n";

			} else if(MCEOracleMarkee->get<Decl>() && MCEDecomMarkee->get<Decl>()){
				cout << "== result decl:" << endl;
				Decl* d1 = const_cast<Decl*>(MCEOracleMarkee->get<Decl>());
				Decl* d2 = const_cast<Decl*>(MCEDecomMarkee->get<Decl>());
				auto result = sec.IsEquivalent(d1, d2);
				cout << "== result:" << result << endl;
				cout << "Diagnostics: " << DiagnosticStream.str() << "\n";
			} else {
				cout << "== result else:" << endl;
			}
			
			string makerIDString = std::to_string(markerID);
			genEval[makerIDString]["oracle"]["ASTNode"] = MCEOracleMarkee->getNodeKind().asStringRef();
			genEval[makerIDString]["oracle"]["children"] = json::array();
			genEval[makerIDString]["oracle"]["properties"] = json::object();;

			genEval[makerIDString]["decom"]["ASTNode"] = MCEDecomMarkee->getNodeKind().asStringRef();
			genEval[makerIDString]["decom"]["children"] = json::array();
			genEval[makerIDString]["decom"]["properties"] = json::object();;
						
			evalSRDeciderP(const_cast<Stmt*>(MCEOracleMarkee->get<Stmt>()), &(genEval[makerIDString]["oracle"]));
			evalSRDeciderP(const_cast<Stmt*>(MCEDecomMarkee->get<Stmt>()), &(genEval[makerIDString]["decom"]));
		}
	}
	
	// Evaluates by traversing through the oracle and then mapping onto the target.
	bool VisitStmt(Stmt *S) {indexProcessor(S); return true;}
		
	const std::unordered_set<Stmt::StmtClass> traversalSkipList = {
		Stmt::IfStmtClass,
		Stmt::WhileStmtClass,
		Stmt::ForStmtClass,
		Stmt::SwitchStmtClass,
//		Stmt::CallExprClass,
		Stmt::CompoundStmtClass
	};

	// experimental
	void evalSRDeciderP(Stmt *node, json *findings){
		bool ct;
		switch(node->getStmtClass()){	// decide node recorder 
			default: 						ct = generalStmtRecorder(node, findings); break;
			case Stmt::DeclStmtClass: 		ct = declStmtRecorder(node, findings); break;
			case Stmt::RecoveryExprClass: 	ct = recoveryExprRecorder(node, findings); break;
			case Stmt::CallExprClass:		ct = callExprRecorder(node, findings); break;
//			case Stmt::IntegerLiteralClass: break;
		}
		if(!ct){return;}
		switch(node->getStmtClass()){	// and	decide traversal continuation
			default:{
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
					evalSRDeciderP(childNode, &storedChild);
				}
				break;
			}
			case Stmt::DeclStmtClass: {
				auto declChildren = llvm::cast<DeclStmt>(node)->decls();
				for(auto childNode:declChildren){
					if (!childNode) {continue;}
					string nodeClassString = childNode->getDeclKindName();
					json child;
					child["ASTNode"] = nodeClassString;
					child["children"] = json::array();
					child["properties"] = json::object();
					(*findings)["children"].push_back(child);
					json &storedChild = (*findings)["children"].back();
					evalDRDeciderP(childNode, &storedChild);
					break;
				}
			}
		}
	}

	void evalDRDeciderP(Decl *node, json *findings){
		bool ct;
		switch(node->getKind()){
			default: generalDeclRecorder(node, findings); break;
			case Decl::Var: 	ct = declVarRecorder(node, findings); break;
		}
		// and	decide traversal continuation
		if(!ct){return;}
		switch(node->getKind()){
			default:{break;}
			case Decl::Var:	
			auto cast_node = llvm::cast<VarDecl>(node);
			auto ExprStmtChild = cast_node->getInit();
			if(!ExprStmtChild){
				(*findings)["properties"]["vGetInit"] = "null";
			} else {
				(*findings)["properties"]["vGetInit"] = ExprStmtChild->getStmtClassName();
				json child;
				child["ASTNode"] = cast_node->getKind();
				child["children"] = json::array();
				child["properties"] = json::object();
				(*findings)["children"].push_back(child);
				json &storedChild = (*findings)["children"].back();
				evalSRDeciderP(ExprStmtChild, &storedChild);
			}
			break;
		}
	}
	
	/* 		Statement processors 	*/
	bool generalStmtRecorder(Stmt *node, json *findings){return true;}
	bool declStmtRecorder(Stmt *node, json *findings){return true;}

	bool recoveryExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<RecoveryExpr>(node);
		(*findings)["properties"]["type"] = cast_node->getType().getAsString();
		return true;
	}

	// voor FunctionDecl aparte entries maken op basis van CallExpr
	bool callExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<CallExpr>(node);
		(*findings)["properties"]["type"] = cast_node->getType().getAsString(); // return type
		auto DC = cast_node->getDirectCallee();
		if(!DC){
			(*findings)["properties"]["direct_callee"] = "unknown";
			return false;
		}
		auto DCName = DC->getNameInfo().getName().getAsString();
		if(DCName == markerFormat["name"].get<string>()){
			(*findings)["properties"]["markee_existence"] = "lost";
			return false;
		}
		(*findings)["properties"]["isVariadic"] = DC->isVariadic();
		(*findings)["properties"]["num_params"] = DC->getNumParams();
		(*findings)["properties"]["num_args"] = cast_node->getNumArgs();
		(*findings)["properties"]["params"] = json::array();
		(*findings)["properties"]["args"] = json::array();

		// If you also want parameter types (requires direct callee)
        for (unsigned i = 0; i < DC->getNumParams(); ++i) {
			QualType T = DC->getParamDecl(i)->getType();
			json paramTypeInfo;
			paramTypeInfo["type"] = T.getAsString();
		    paramTypeInfo["is_const"] = T.isConstQualified();
		    paramTypeInfo["is_pointer"] = T->isPointerType();
            (*findings)["properties"]["params"].push_back(DC->getParamDecl(i)->getType().getAsString());
        }
		
		for (unsigned i = 0; i < cast_node->getNumArgs(); ++i) {
			QualType T = cast_node->getArg(i)->getType();
		    json argTypeInfo;
		    argTypeInfo["type"] = T.getAsString();
		    argTypeInfo["is_const"] = T.isConstQualified();
		    argTypeInfo["is_pointer"] = T->isPointerType();
			((*findings)["properties"]["args"]).push_back(argTypeInfo);
		}
		return false;
	}
	
	bool forStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<ForStmt>(node);
		
//		json props;
//		    
//	    // Part 1: Initialization
//	    if (Stmt *Init = FS->getInit()) {
//	        props["init"] = serializeStmt(Init);  // or your custom processing
//	    }
//	    
//	    // Part 2: Condition variable (C99 style)
//	    if (VarDecl *CondVar = FS->getConditionVariable()) {
//	        props["cond_var"] = {
//	            {"name", CondVar->getNameAsString()},
//	            {"type", CondVar->getType().getAsString()}
//	        };
//	    }
//	    
//	    // Part 3: Condition expression
//	    if (Expr *Cond = FS->getCond()) {
//	        props["condition"] = serializeExpr(Cond);
//	    }
//	    
//	    // Part 4: Increment
//	    if (Stmt *Inc = FS->getInc()) {
//	        props["increment"] = serializeStmt(Inc);
//	    }
//	    
//	    // DO NOT process the body (FS->getBody() or the last child)
//	    (*findings)["properties"] = props;
		
		return false;
	}
	
	/* 		Declaration processors 	*/

	// entry should give diagnostic effects
	bool generalDeclRecorder(Decl *node, json *findings){
		(*findings)["properties"]["TODO"] = true;
		return false;
	}

	bool declVarRecorder(Decl *node, json *findings){
		auto cast_node = llvm::cast<VarDecl>(node);
		(*findings)["properties"]["type"] = cast_node->getType().getAsString();
		// if parent context is necessary we do it here else we return true
		return false;
	}
};

