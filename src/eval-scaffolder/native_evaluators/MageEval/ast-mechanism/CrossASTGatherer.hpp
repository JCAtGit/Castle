
#include <cstddef>
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTStructuralEquivalence.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/AST/Type.h"
#include "clang/Basic/SourceManager.h"
#include "llvm/Support/Casting.h"
#include "llvm/Support/raw_ostream.h"
#include "../ast-mechanism/ast-helper.hpp"


using namespace nlohmann;
using namespace clang;
using namespace std;

class CrossASTGatherer: public RecursiveASTVisitor<CrossASTGatherer> {
	private:
	json &markerFormat, &findings, &specEval, &markerEval, &humanEval;
	bool& nameEncodedMarkers;
	
	ASTContext *oracleAST, *decomAST;
	SourceManager *oracleSM, *decomSM;
	
	unordered_map<int64_t, CallExpr*> decomMarkerMap;
	unordered_map<int64_t, CallExpr*> oracleMarkerMap;
	
	vector<Stmt*> parentStack;
	unordered_map<int64_t, Stmt*> decomMarkerParentMap;
	unordered_map<int64_t, Stmt*> oracleMarkerParentMap;

	unordered_map<int64_t, Stmt*> decomMarkeeMap;
	unordered_map<int64_t, Stmt*> oracleMarkeeMap;

	int proc_mode = 0; // 0 = oracle, 1 = decom
	void (CrossASTGatherer::*selectedProcessor)(Stmt *S);
	ASTContext* selectedAST;
	SourceManager* selectedSM;
	string selectedContextString, magestring;
	unordered_map<int64_t, CallExpr*>* 	selectedMarkerMap;
	unordered_map<int64_t, Stmt*>* 		selectedMarkerParentMap;
	unordered_map<int64_t, Stmt*>* 		selectedMarkeeMap;

	// Structural equivalence functionality
//	StructuralEquivalenceContext::NonEquivalentDeclSet neds;
//	StructuralEquivalenceContext sec;
	
	public:	
	CrossASTGatherer(
		json& markerFormat, json& findings, json& specEval, json& markerEval, json& humanEval,
		ASTContext *oracleAST, ASTContext *decomAST, 
		bool& nameEncodedMarkers
	): 
	markerFormat(markerFormat),	findings(findings), specEval(specEval), markerEval(markerEval), humanEval(humanEval),
	oracleAST(oracleAST), decomAST(decomAST), 
	nameEncodedMarkers(nameEncodedMarkers),
	
	oracleSM(&oracleAST->getSourceManager()), decomSM(&decomAST->getSourceManager())
	{
		magestring = markerFormat["name"].get<string>();;
	}
	
	CrossASTGatherer(
		json& markerFormat, json& findings, json& specEval, json& markerEval, json& humanEval, 
		bool& nameEncodedMarkers
	): markerFormat(markerFormat), findings(findings), specEval(specEval), markerEval(markerEval), humanEval(humanEval), nameEncodedMarkers(nameEncodedMarkers)
	{magestring = markerFormat["name"].get<string>();}
	
	void setDecomContext(ASTContext *otherAST){
		decomAST = otherAST;
		selectedSM = &(otherAST->getSourceManager());
	}
	
	void setOracleContext(ASTContext *otherAST){
		oracleAST = otherAST;
		selectedSM = &(otherAST->getSourceManager());
	}
	
	void selectMode(int proc_mode){
		switch (proc_mode) {
			case 0:
			selectedAST = decomAST;
			selectedSM = decomSM;
			selectedMarkerMap = &decomMarkerMap;
			selectedMarkerParentMap = &decomMarkerParentMap;
			selectedContextString = "decom";
			break;
			case 1:
			selectedAST = oracleAST;
			selectedSM = oracleSM;
			selectedMarkerMap = &oracleMarkerMap;
			selectedMarkerParentMap = &oracleMarkerParentMap;
			selectedContextString = "oracle";
			break;			
			default:
			selectedProcessor = NULL;
			selectedContextString = "none";
			break;
		}
	}
	
	// Sommige DC dingen nu dubbel
	int getMageIndex(CallExpr* markerCE){
		auto DC = markerCE->getDirectCallee();
		if (!DC) return -1;
		string namestring = DC->getNameInfo().getName().getAsString();

		if (namestring.compare(0, magestring.length(), magestring) != 0){return -1;} 		
		if (markerCE->getNumArgs() > 0 && !nameEncodedMarkers){
			auto IL = dyn_cast<IntegerLiteral>(markerCE->getArg(0)->IgnoreImpCasts());
			if (IL) {
				llvm::APInt Value = IL->getValue();
				return Value.getSExtValue();
			}
		} else if(nameEncodedMarkers){
			string postfix = namestring.substr(magestring.length());
			if (postfix.empty() || !std::all_of(postfix.begin(), postfix.end(), ::isdigit)) {return -1;}
			return std::stoi(postfix);
		}
		return -1;
	}
	
	void indexProcessor(CallExpr *CE){
		if (selectedSM->isInMainFile(CE->getBeginLoc())){				
			int mageIndex = getMageIndex(CE);
			if(mageIndex == -1){return;}
			if (parentStack.size() < 2) {return;}

			(*selectedMarkerMap)[mageIndex] = CE;			
		    Stmt *parent = parentStack[parentStack.size() - 2];  // immediate parent
		    (*selectedMarkerParentMap)[mageIndex] = parent;
			auto siblings = getChildrenNodes(parent);
			auto CESibIndex = findNodeSiblingIndex(siblings, DynTypedNode::create(*CE));
			auto CEMarkee = getNextSiblingFromList(siblings, CESibIndex);
			auto mageIndexString = std::to_string(mageIndex);
			if(CEMarkee){
				findings[mageIndexString][selectedContextString]["ASTNode"] = CEMarkee->getNodeKind().asStringRef();
				findings[mageIndexString][selectedContextString]["Source"] = 
					llvm::sys::path::filename(selectedSM->getFilename(CE->getBeginLoc())).str() 	+ " (" + 
					std::to_string(selectedSM->getSpellingLineNumber(CE->getBeginLoc()))					+ ", "+
				 	std::to_string(selectedSM->getSpellingColumnNumber(CE->getBeginLoc()))				+ ") " ;
				findings[mageIndexString][selectedContextString]["children"] = json::array();
				findings[mageIndexString][selectedContextString]["properties"] = json::object();
				evalSRDeciderP(const_cast<Stmt*>(CEMarkee->get<Stmt>()), &(findings[mageIndexString][selectedContextString]));
			} else {
				// TODO: report to dev
			}
		}
	}
	
	void announceFindings(){
		// TODO: oracle decom aanwezigheid verifieren 
		cout << "OracleMap:" << oracleMarkerMap.size() << " - DecomMap:" << decomMarkerMap.size() << "\n"; 
		cout << "OracleParentMap:" << oracleMarkerParentMap.size() << " - DecomParentMap:" << decomMarkerParentMap.size() << "\n"; 
	}
	
	bool TraverseStmt(Stmt *S) {
//		if (!S) return true;
        parentStack.push_back(S);
        bool result = RecursiveASTVisitor<CrossASTGatherer>::TraverseStmt(S);
        parentStack.pop_back();
        return result;
	}
	
	bool VisitCallExpr(CallExpr *CE){
		indexProcessor(CE); 
		return true;
	}

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
			case Stmt::SwitchStmtClass:		ct = switchStmtRecorder(node,findings); break;
//			case Stmt::IntegerLiteralClass: break;
		}
		if(!ct){return;}
	}

	// misschien niet nodig
	void evalDRDeciderP(Decl *node, json *findings){
		bool ct;
		switch(node->getKind()){
			default: generalDeclRecorder(node, findings); break;
			case Decl::Var: 	ct = declVarRecorder(node, findings); break;
		}
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
		(*findings)["properties"]["has_markee"] = false;

		if (!isa<Expr>(node)) {
			auto cast_node = llvm::cast<Expr>(node);
//			(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();
		}
		return false;
	}
	
	bool declStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<DeclStmt>(node);
		(*findings)["properties"]["has_markee"] = true;
		(*findings)["properties"]["single_decl"] = cast_node->isSingleDecl();
		auto decls = cast_node->decls();
		for(auto uncast_decl: decls){
	        switch (uncast_decl->getKind()) {
	        case clang::Decl::Var: {
	            auto *VD = cast<clang::VarDecl>(uncast_decl);
				recordQTypeInfo(VD->getType(), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        case clang::Decl::Function: {
	            auto *FD = cast<clang::FunctionDecl>(uncast_decl);
				recordQTypeInfo(FD->getType(), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        case clang::Decl::Typedef: {
	            auto *TD = cast<clang::TypedefDecl>(uncast_decl);
				recordQTypeInfo(TD->getUnderlyingType(), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        case clang::Decl::Record: {
	            auto *RD = cast<clang::RecordDecl>(uncast_decl);
				recordQTypeInfo(selectedAST->getTypeDeclType(RD), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        case clang::Decl::Enum: {
	            auto *ED = cast<clang::EnumDecl>(uncast_decl);
				recordQTypeInfo(selectedAST->getTypeDeclType(ED), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        default:
	            // TODO: report to diagnostics
	            break;
	    	}
		}
		return false;
	}

	bool recoveryExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<RecoveryExpr>(node);
		(*findings)["properties"]["has_markee"] = true;
//		(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();

		recordQTypeInfo(cast_node->getType(), (*findings)["properties"]["types"]);
		for(auto reChild:cast_node->children()){
			switch(reChild->getStmtClass()){
				default: break;
				case Stmt::DeclRefExprClass: break;
			}
		}
		return false;
	}

	// Voor FunctionDecl aparte entries maken op basis van CallExpr
	bool callExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<CallExpr>(node);
		(*findings)["properties"]["has_markee"] = false;

		(*findings)["properties"]["return_type"] = cast_node->getType().getAsString();
		(*findings)["properties"]["has_side_effects"] = cast_node->HasSideEffects(*selectedAST);
		(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();
		
		auto DC = cast_node->getDirectCallee();
		if(!DC){
			(*findings)["properties"]["has_direct_callee"] = false;
			// TODO: what if DC is null, then the eval will produce errors currently
			return false;
		}
		(*findings)["properties"]["has_direct_callee"] = true;
		
		auto DCName = DC->getNameInfo().getName().getAsString();
		if(DCName == markerFormat["name"].get<string>()){
			return false;
		}
		(*findings)["properties"]["has_markee"] = true;

		(*findings)["properties"]["direct_callee_name"] = DCName;
		(*findings)["properties"]["is_variadic"] = DC->isVariadic();
		(*findings)["properties"]["num_params"] = DC->getNumParams();
		(*findings)["properties"]["num_args"] = cast_node->getNumArgs();
		(*findings)["properties"]["params"] = json::array();
		(*findings)["properties"]["args"] = json::array();

        for (unsigned i = 0; i < DC->getNumParams(); ++i) {
			recordQTypeInfo(DC->getParamDecl(i)->getType(), (*findings)["properties"]["param_types"]);
        }
		
		for (unsigned i = 0; i < cast_node->getNumArgs(); ++i) {
			recordQTypeInfo(cast_node->getArg(i)->getType(), (*findings)["properties"]["arg_types"]);
		}
		return false;
	}
	

	bool forStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<ForStmt>(node);
		(*findings)["properties"]["has_markee"] = true;

		auto forInitStmt = cast_node->getInit();
		if(forInitStmt){
			(*findings)["properties"]["has_init"] = true;
			(*findings)["properties"]["init_stmt_class"] = forInitStmt->getStmtClassName();
			switch(forInitStmt->getStmtClass()){
				default: 
					if (isa<Expr>(forInitStmt)) {
						auto cast_node = llvm::cast<Expr>(forInitStmt);
//						(*findings)["properties"]["init_contains_errors"] = cast_node->containsErrors();
						recordQTypeInfo(cast_node->getType(), (*findings)["properties"]["init_types"]);
					} else {
						// TODO: report to diagnostics
					}
				break;
				case Stmt::DeclStmtClass:{
					(*findings)["properties"]["init_types"] = json::array();
					auto forInitDeclStmt = llvm::cast<DeclStmt>(forInitStmt);
					(*findings)["properties"]["init_single_decl"] = forInitDeclStmt->isSingleDecl();
					auto forInitVarDecls = forInitDeclStmt->decls();
					for(auto uncast_decl: forInitVarDecls){
						switch (uncast_decl->getKind()) {
					        case clang::Decl::Var: {
					            auto *VD = cast<clang::VarDecl>(uncast_decl);
								recordQTypeInfo(VD->getType(), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Function: {
					            auto *FD = cast<clang::FunctionDecl>(uncast_decl);
								recordQTypeInfo(FD->getType(), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Typedef: {
					            auto *TD = cast<clang::TypedefDecl>(uncast_decl);
								recordQTypeInfo(TD->getUnderlyingType(), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Record: {
					            auto *RD = cast<clang::RecordDecl>(uncast_decl);
								recordQTypeInfo(selectedAST->getTypeDeclType(RD), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Enum: {
					            auto *ED = cast<clang::EnumDecl>(uncast_decl);
								recordQTypeInfo(selectedAST->getTypeDeclType(ED), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        default:
					            // TODO: report to diagnostics
					            break;
					    	}
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
			(*findings)["properties"]["cond_var_stmt_class"] = condVar->getDeclKindName();
			recordQTypeInfo(condVar->getType(), (*findings)["properties"]["cond_var_types"]);
		} else {
			(*findings)["properties"]["has_cond_var"] = false;
		}
		
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			(*findings)["properties"]["cond_stmt_class"] = conditionExpr->getStmtClassName();
//			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		
		auto incExpr = cast_node->getInc();
		if(incExpr){
			(*findings)["properties"]["has_incr"] = true;
			(*findings)["properties"]["incr_stmt_class"] = incExpr->getStmtClassName();
//			(*findings)["properties"]["incr_has_errors"] = conditionExpr->containsErrors();
			recordQTypeInfo(incExpr->getType(), (*findings)["properties"]["incr_expr_types"]);
		} else {
			(*findings)["properties"]["has_incr"] = false;
		}
		return false;
	}
	
	bool whileStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<WhileStmt>(node);
		(*findings)["properties"]["has_markee"] = true;

		auto condVar = cast_node->getConditionVariable();
		if(condVar){
			(*findings)["properties"]["has_cond_var"] = true;
			recordQTypeInfo(condVar->getType(), (*findings)["properties"]["cond_var_types"]);
		} else {
			(*findings)["properties"]["has_cond_var"] = false;
		}
		
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			(*findings)["properties"]["cond_stmt_class"] = conditionExpr->getStmtClassName();
//			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		return false;
	}
	
	bool doStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<DoStmt>(node);
		(*findings)["properties"]["has_markee"] = true;
		
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			(*findings)["properties"]["cond_stmt_class"] = conditionExpr->getStmtClassName();
//			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		return false;
	}
	
	bool ifStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<IfStmt>(node);
		(*findings)["properties"]["has_markee"] = true;

		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["cond_stmt_class"] = conditionExpr->getStmtClassName();
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			// report to diagnostics, shouldnt happen for C code.
		}
		return false;
	}
	
	/* 			Statements Recorders - Expr		*/
	bool binaryOperatorRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<BinaryOperator>(node);
		(*findings)["properties"]["has_markee"] = true;
//		(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();

		(*findings)["properties"]["operator"] = cast_node->getOpcodeStr();
		(*findings)["properties"]["operator_opcode"] = cast_node->getOpcode();
		recordQTypeInfo(cast_node->getType(), (*findings)["properties"]["operator_types"]);
		
		auto lhs_node =	cast_node->getLHS();
		(*findings)["properties"]["lhs_stmt_class"] = lhs_node->getStmtClassName();
//		(*findings)["properties"]["lhs_contains_errors"] = lhs_node->containsErrors();
		recordQTypeInfo(lhs_node->getType(), (*findings)["properties"]["lhs_types"]);

		auto rhs_node =	cast_node->getRHS();
		(*findings)["properties"]["rhs_stmt_class"] = rhs_node->getStmtClassName();
//		(*findings)["properties"]["rhs_contains_errors"] = rhs_node->containsErrors();
		recordQTypeInfo(rhs_node->getType(), (*findings)["properties"]["rhs_types"]);

		return false;
	}
	
	bool switchStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<SwitchStmt>(node);
		(*findings)["properties"]["has_markee"] = true;
				
		auto switchInitStmt = cast_node->getInit();
		if(switchInitStmt){
			(*findings)["properties"]["has_init"] = true;
			(*findings)["properties"]["init_stmt_class"] = switchInitStmt->getStmtClassName();
			switch(switchInitStmt->getStmtClass()){
				default: 
					if (isa<Expr>(switchInitStmt)) {
						auto cast_node = llvm::cast<Expr>(switchInitStmt);
//						(*findings)["properties"]["init_contains_errors"] = cast_node->containsErrors();
						recordQTypeInfo(cast_node->getType(), (*findings)["properties"]["init_types"]);
					} else {
						// TODO: report to diagnostics
					}
				break;
				case Stmt::DeclStmtClass:{
					(*findings)["properties"]["init_types"] = json::array();
					auto forInitDeclStmt = llvm::cast<DeclStmt>(switchInitStmt);
					(*findings)["properties"]["init_single_decl"] = forInitDeclStmt->isSingleDecl();
					auto forInitVarDecls = forInitDeclStmt->decls();
					for(auto uncast_decl: forInitVarDecls){
						switch (uncast_decl->getKind()) {
					        case clang::Decl::Var: {
					            auto *VD = cast<clang::VarDecl>(uncast_decl);
								recordQTypeInfo(VD->getType(), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Function: {
					            auto *FD = cast<clang::FunctionDecl>(uncast_decl);
								recordQTypeInfo(FD->getType(), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Typedef: {
					            auto *TD = cast<clang::TypedefDecl>(uncast_decl);
								recordQTypeInfo(TD->getUnderlyingType(), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Record: {
					            auto *RD = cast<clang::RecordDecl>(uncast_decl);
								recordQTypeInfo(selectedAST->getTypeDeclType(RD), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        case clang::Decl::Enum: {
					            auto *ED = cast<clang::EnumDecl>(uncast_decl);
								recordQTypeInfo(selectedAST->getTypeDeclType(ED), (*findings)["properties"]["init_types"]);
					            break;
					        }
					        default:
					            // TODO: report to diagnostics
					            break;
					    	}
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
			(*findings)["properties"]["cond_var_stmt_class"] = condVar->getDeclKindName();
			recordQTypeInfo(condVar->getType(), (*findings)["properties"]["cond_var_types"]);
		} else {
			(*findings)["properties"]["has_cond_var"] = false;
		}
		
		auto conditionExpr = cast_node->getCond();
		if(conditionExpr){
			(*findings)["properties"]["has_cond_expr"] = true;
			(*findings)["properties"]["cond_stmt_class"] = conditionExpr->getStmtClassName();
//			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
		}
		
		auto switchCaseList = cast_node->getSwitchCaseList();
		(*findings)["properties"]["switch_cases"] = json::array();
		if(switchCaseList){
			(*findings)["properties"]["has_switch_cases"] = true;
//			hasFallthroughWidth(cast_node, (*findings)["properties"]["switch_cases"]);

			while(switchCaseList != nullptr){
				json caseInfo = json::object();
				caseInfo["case_stmt_class"] = switchCaseList->getStmtClassName();
				caseInfo["case_substmt_class"] = switchCaseList->getSubStmt()->getStmtClassName();
				(*findings)["properties"]["switch_cases"].push_back(caseInfo);
				switchCaseList = switchCaseList->getNextSwitchCase();
			}
		} else {
			(*findings)["properties"]["has_switch_cases"] = false;
		}		
		return false;
	}
	
	/* 			Declaration Recorders 			*/

	// TODO: entries should give diagnostic effects, also all decls are still WIP.
	
	bool generalDeclRecorder(Decl *node, json *findings){
		(*findings)["properties"]["TODO"] = true;
		return false;
	}

	bool declVarRecorder(Decl *node, json *findings){
		auto cast_node = llvm::cast<VarDecl>(node);
		recordQTypeInfo(cast_node->getType(), (*findings)["properties"]["types"]);
		return false;
	}
};

