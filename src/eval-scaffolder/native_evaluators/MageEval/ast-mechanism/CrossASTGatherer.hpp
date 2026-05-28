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
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

class CrossASTGatherer: public RecursiveASTVisitor<CrossASTGatherer> {
	private:
	json &markerFormat, &findings, &specEval, &markerEval, &humanEval;
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
		json& markerFormat, json& findings, json& specEval, json& markerEval, json& humanEval,
		ASTContext &oracleAST, ASTContext &decomAST): 
	markerFormat(markerFormat),	findings(findings), specEval(specEval), markerEval(markerEval), humanEval(humanEval),
	oracleAST(oracleAST), decomAST(decomAST),
	
	oracleSM(oracleAST.getSourceManager()), decomSM(decomAST.getSourceManager()), 
	sec(
		StructuralEquivalenceContext(
			oracleAST.getLangOpts(),
		    oracleAST,
		    decomAST,
		    neds,
		    StructuralEquivalenceKind::Default,
		    false, 						//StrictTypeSpelling
		    true, 								// Complain, structural errors emit straight to your TextDiagnosticPrinter
		    true					//ErrorOnTagTypeMismatch
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
			} else if(MCEOracleMarkee->get<Decl>() && MCEDecomMarkee->get<Decl>()){
				cout << "== result decl:" << endl;
				Decl* d1 = const_cast<Decl*>(MCEOracleMarkee->get<Decl>());
				Decl* d2 = const_cast<Decl*>(MCEDecomMarkee->get<Decl>());

				auto result = sec.IsEquivalent(d1, d2);
				cout << "== result:" << result << endl;
			} else {
				cout << "== result else:" << endl;
			}
			
			string makerIDString = std::to_string(markerID);
			findings[makerIDString]["oracle"]["ASTNode"] = MCEOracleMarkee->getNodeKind().asStringRef();
			findings[makerIDString]["oracle"]["children"] = json::array();
			findings[makerIDString]["oracle"]["properties"] = json::object();;

			findings[makerIDString]["decom"]["ASTNode"] = MCEDecomMarkee->getNodeKind().asStringRef();
			findings[makerIDString]["decom"]["children"] = json::array();
			findings[makerIDString]["decom"]["properties"] = json::object();;
						
			evalSRDeciderP(const_cast<Stmt*>(MCEOracleMarkee->get<Stmt>()), &(findings[makerIDString]["oracle"]));
			evalSRDeciderP(const_cast<Stmt*>(MCEDecomMarkee->get<Stmt>()), &(findings[makerIDString]["decom"]));
		}
	}
	
	// Evaluates by traversing through the oracle and then mapping onto the target.
	bool VisitStmt(Stmt *S) {indexProcessor(S); return true;}
		
	const std::unordered_set<Stmt::StmtClass> traversalSkipList = {
//		Stmt::ImplicitCastExprClass,
//		Stmt::DeclRefExprClass,
		Stmt::BinaryOperatorClass,
		Stmt::CallExprClass,
		Stmt::IfStmtClass,
		Stmt::WhileStmtClass,
		Stmt::DoStmtClass,
		Stmt::ForStmtClass,
		Stmt::SwitchStmtClass,
		Stmt::CompoundStmtClass
	};

	// experimental
	void evalSRDeciderP(Stmt *node, json *findings){
		bool ct;
		switch(node->getStmtClass()){
			default: 						ct = generalStmtRecorder(node, findings); break;
			case Stmt::DeclStmtClass: 		ct = declStmtRecorder(node, findings); break;
			case Stmt::RecoveryExprClass: 	ct = recoveryExprRecorder(node, findings); break;
			case Stmt::CallExprClass:		ct = callExprRecorder(node, findings); break;
			case Stmt::ForStmtClass:		ct = forStmtRecorder(node, findings); break;
			case Stmt::WhileStmtClass:		ct = whileStmtRecorder(node,findings); break;
			case Stmt::DoStmtClass:			ct = doStmtRecorder(node, findings); break;
			case Stmt::IfStmtClass:			ct = ifStmtRecorder(node,findings); break;
			case Stmt::BinaryOperatorClass:	ct = binaryOperatorRecorder(node,findings); break;
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
	
	/* 		Statement Recorders - Non-Expr		*/
	bool generalStmtRecorder(Stmt *node, json *findings){
		if (!isa<Expr>(node)) {
			auto cast_node = llvm::cast<Expr>(node);
			(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();
		}
		return true;
	}
	
	bool declStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<DeclStmt>(node);
		(*findings)["properties"]["single_decl"] = cast_node->isSingleDecl();
		auto decls = cast_node->decls();
		for(auto uncast_varDecl: decls){
			auto cast_varDecl = llvm::cast<VarDecl>(uncast_varDecl);
			recordTypeInfo(cast_varDecl->getType(), (*findings)["properties"]["init_types"]);
		}
		return false;
	}

	bool recoveryExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<RecoveryExpr>(node);
		recordTypeInfo(cast_node->getType(), (*findings)["properties"]["types"]);
		for(auto reChild:cast_node->children()){
			switch(reChild->getStmtClass()){
				default: break;
				case Stmt::DeclRefExprClass: break;
			}
		}
		return true;
	}

	// voor FunctionDecl aparte entries maken op basis van CallExpr
	bool callExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<CallExpr>(node);
		(*findings)["properties"]["return_type"] = cast_node->getType().getAsString();
		(*findings)["properties"]["has_side_effects"] = cast_node->HasSideEffects(*selectedAST);
		(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();
		
		auto DC = cast_node->getDirectCallee();
		if(!DC){
			(*findings)["properties"]["has_direct_callee"] = false;
			return false;
		}
		(*findings)["properties"]["has_direct_callee"] = true;
		auto DCName = DC->getNameInfo().getName().getAsString();
		if(DCName == markerFormat["name"].get<string>()){
			(*findings)["properties"]["has_markee"] = false;
			return false;
		}
		(*findings)["properties"]["direct_callee_name"] = DCName;
		(*findings)["properties"]["isVariadic"] = DC->isVariadic();
		(*findings)["properties"]["num_params"] = DC->getNumParams();
		(*findings)["properties"]["num_args"] = cast_node->getNumArgs();
		(*findings)["properties"]["params"] = json::array();
		(*findings)["properties"]["args"] = json::array();

        for (unsigned i = 0; i < DC->getNumParams(); ++i) {
			recordTypeInfo(DC->getParamDecl(i)->getType(), (*findings)["properties"]["params"]);
        }
		
		for (unsigned i = 0; i < cast_node->getNumArgs(); ++i) {
			recordTypeInfo(cast_node->getArg(i)->getType(), (*findings)["properties"]["args"]);
		}
		return false;
	}
	
	bool forStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<ForStmt>(node);
		auto forInitStmt = cast_node->getInit();
		if(forInitStmt){
			(*findings)["properties"]["has_init"] = true;
			switch(forInitStmt->getStmtClass()){
				default: 
					(*findings)["properties"]["init_is_expr"] = isa<Expr>(node);
					if (isa<Expr>(node)) {
						auto cast_node = llvm::cast<Expr>(node);
						(*findings)["properties"]["init_contains_errors"] = cast_node->containsErrors();
						recordTypeInfo(cast_node->getType(), (*findings)["properties"]["init_expr_types"]);
					}			
				break;
				case Stmt::DeclStmtClass:{
					(*findings)["properties"]["init_is_expr"] = false;
					(*findings)["properties"]["init_types"] = json::array();
					auto forInitDeclStmt = llvm::cast<DeclStmt>(forInitStmt);
					(*findings)["properties"]["init_single_decl"] = forInitDeclStmt->isSingleDecl();
					auto forInitVarDecls = forInitDeclStmt->decls();
					for(auto uncast_varDecl: forInitVarDecls){
						auto cast_varDecl = llvm::cast<VarDecl>(uncast_varDecl);
						recordTypeInfo(cast_varDecl->getType(),(*findings)["properties"]["init_types"]);
					}
					break;
				}
			}
		} else {
			(*findings)["properties"]["has_init"] = false;
		}
		
		auto condVar = cast_node->getConditionVariable();
		if(condVar){
			(*findings)["properties"]["has_cond_var"] = true;
			recordTypeInfo(condVar->getType(), (*findings)["properties"]["cond_var_types"]);
		} else {
			(*findings)["properties"]["has_cond_var"] = false;
		}
		
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		
		auto incExpr = cast_node->getInc();
		if(incExpr){
			(*findings)["properties"]["has_incr"] = true;
			(*findings)["properties"]["incr_has_errors"] = conditionExpr->containsErrors();
			recordTypeInfo(incExpr->getType(), (*findings)["properties"]["incr_expr_types"]);
		} else {
			(*findings)["properties"]["has_incr"] = false;
		}
		return false;
	}
	
	bool whileStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<WhileStmt>(node);
		
		auto condVar = cast_node->getConditionVariable();
		if(condVar){
			(*findings)["properties"]["has_cond_var"] = true;
			recordTypeInfo(condVar->getType(), (*findings)["properties"]["cond_var_types"]);
		} else {
			(*findings)["properties"]["has_cond_var"] = false;
		}
		
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		return false;
	}
	
	bool doStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<DoStmt>(node);
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			recordTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		return false;
	}
	
	bool ifStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<IfStmt>(node);
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			recordTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		return false;
	}
	
	
	/* 			Statements Recorders - Expr		*/
	bool binaryOperatorRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<BinaryOperator>(node);
		(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();
		(*findings)["properties"]["operator"] = cast_node->getOpcodeStr();
		(*findings)["properties"]["operator_opcode"] = cast_node->getOpcode();
		recordTypeInfo(cast_node->getType(), (*findings)["properties"]["operator_types"]);
		
		auto lhs_node =	cast_node->getLHS();
		(*findings)["properties"]["LHS_contains_errors"] = lhs_node->containsErrors();
		recordTypeInfo(lhs_node->getType(), (*findings)["properties"]["LHS_types"]);

		auto rhs_node =	cast_node->getRHS();
		(*findings)["properties"]["RHS_contains_errors"] = rhs_node->containsErrors();
		recordTypeInfo(rhs_node->getType(), (*findings)["properties"]["LHS_types"]);

		return false;
	}
	
	/* 			Declaration Recorders 			*/

	// entry should give diagnostic effects
	bool generalDeclRecorder(Decl *node, json *findings){
		(*findings)["properties"]["TODO"] = true;
		return false;
	}

	bool declVarRecorder(Decl *node, json *findings){
		auto cast_node = llvm::cast<VarDecl>(node);
		recordTypeInfo(cast_node->getType(), (*findings)["properties"]["types"]);
		return false;
	}
};

