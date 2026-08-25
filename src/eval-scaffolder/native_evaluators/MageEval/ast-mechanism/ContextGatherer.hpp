
#include <nlohmann/json.hpp>
#include <string>
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "../ast-mechanism/ast-helper.hpp"

using namespace nlohmann;
using namespace clang;
using namespace std;

/**
 *	A Class that is responsible for gathering findings per context independently and storing these in a shared object.
 * 	Currently parses both ast's without the inclusion of a library.
 *	TODO: 
 *		- Record findings explicitly to dev files.
 *		- Include library during ast parsing of oracle, however the necessary libraries that need to be included depend on the build target.
 *			- analyse and/or record build details instead of binary extraction?
 *			- derive necessary libraries from build details and include them
 */
class ContextGatherer: public RecursiveASTVisitor<ContextGatherer> {
	private:
	json 								&markerFormat, &findings;
	bool								&nameEncodedMarkers;
	
	ASTContext 							*oracleAST, *decomAST;
	SourceManager 						*oracleSM, 	*decomSM;
	
	vector<Stmt*> 						parentStack;
	
	ASTContext							*selectedAST;
	SourceManager						*selectedSM;
	
	int 								context_mode = 0; // 0 = oracle, 1 = decom

	void 								(ContextGatherer::*selectedProcessor)(Stmt *S);
	
	string 								selectedContextString, 
										magestring;
	
	public:	
	ContextGatherer(
		json		&markerFormat, 
		json		&findings,
		ASTContext 	*oracleAST, 
		ASTContext 	*decomAST, 
		bool 		&nameEncodedMarkers
	): 
	markerFormat(markerFormat),	
	findings(findings),
	oracleAST(oracleAST), 
	decomAST(decomAST), 
	nameEncodedMarkers(nameEncodedMarkers),
	
	oracleSM(&oracleAST->getSourceManager()), 
	decomSM(&decomAST->getSourceManager())
	{
		magestring = markerFormat["name"].get<string>();;
	}
	
	ContextGatherer(
		json &markerFormat, 
		json &findings, 
		bool &nameEncodedMarkers
	): 
	markerFormat(markerFormat), 
	findings(findings), 
	nameEncodedMarkers(nameEncodedMarkers)
	{
		magestring = markerFormat["name"].get<string>();
	}
	
	void setDecomContext(ASTContext *otherAST){
		decomAST = otherAST;
		selectedSM = &(otherAST->getSourceManager());
	}
	
	void setOracleContext(ASTContext *otherAST){
		oracleAST = otherAST;
		selectedSM = &(otherAST->getSourceManager());
	}
	
	void setFindingsObject(json &findings_ref){
		findings = findings_ref;
	}
	
	/**
	 *	Selects which context is being recorded depending on the mode (oracle or decom).
	 *	@param context_mode 0 = decom, 1 =  oracle.
	 */
	void selectMode(int context_mode){
		switch (context_mode) {
			case 0:
			selectedAST = decomAST;
			selectedSM = decomSM;
			selectedContextString = "decom";
			break;
			case 1:
			selectedAST = oracleAST;
			selectedSM = oracleSM;
			selectedContextString = "oracle";
			break;			
			default:
			selectedProcessor = NULL;
			selectedContextString = "none";
			break;
		}
	}
	
	/**
	 *	TODO: 
	 *		add detection for when debug information is disabled or absent.
	 *		decouple nameEncodedMarkers, magestring from function definition.
	 *
	 *	Heuristic function to determine the identity of a marker function from a CallExpr.
	 *	When nameEncodedMarkers are enable, indexes will extracted from the name instead.
	 *	@param potentialMarker Investigated CallExpr for marker characteristics.
	 *	@return -1 if unable to be identified as a marker.
	 */
	int getMageIndex(CallExpr* potentialMarker){
		auto DC = potentialMarker->getDirectCallee();
		if (!DC) return -1;
		string namestring = DC->getNameInfo().getName().getAsString();

		if (namestring.compare(0, magestring.length(), magestring) != 0){return -1;} 		
		if (potentialMarker->getNumArgs() > 0 && !nameEncodedMarkers){
			auto IL = dyn_cast<IntegerLiteral>(potentialMarker->getArg(0)->IgnoreImpCasts());
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
	
	/**
	 *	For a given CallExpr, checks if it is a marker call.
	 *	If it is found to be a marker, then the potential markee will be fetched.
	 *	Then based on the type of the markee, the appropriate recorder will be selected.
	 *	@param CE CallExpr that is selected for procession.
	 */
	void indexProcessor(CallExpr *CE){
		if (selectedSM->isInMainFile(CE->getBeginLoc())){
			int mageIndex = getMageIndex(CE);
			if(mageIndex == -1){return;}
			if (parentStack.size() < 2) {return;}

//			(*selectedMarkerMap)[mageIndex] = CE;			
		    Stmt *parent = parentStack[parentStack.size() - 2];  // immediate parent, size-1 is the statement itself.
//		    (*selectedMarkerParentMap)[mageIndex] = parent;
			auto siblings = getChildrenNodes(parent);
			auto CESibIndex = findNodeSiblingIndex(siblings, DynTypedNode::create(*CE));
			auto CEMarkee = getNextSiblingFromList(siblings, CESibIndex);
			auto mageIndexString = std::to_string(mageIndex);

			if(CEMarkee){ // for some unevaluated markee, which we will later determine to be a legit markee or not.
				json markerFinding = json::object();
				markerFinding["ASTNode"] = CEMarkee->getNodeKind().asStringRef();
				markerFinding["Source"] = 
					selectedSM->getFilename(CE->getBeginLoc()).str() 					+ " (" + 
					to_string(selectedSM->getSpellingLineNumber(CE->getBeginLoc()))		+ ", "+
				 	to_string(selectedSM->getSpellingColumnNumber(CE->getBeginLoc()))		+ ") " ;
//				markerFinding["children"] = json::array(); // wordt nu niks mee gedaan
				markerFinding["properties"] = json::object();
				
				evalSRDeciderP(const_cast<Stmt*>(CEMarkee->get<Stmt>()), &markerFinding);
				findings[mageIndexString][selectedContextString].push_back(markerFinding);
			} else {
				// TODO: report to dev if not already accounted for
			}
		}
	}
	
	/**
	 *	The ClangAST parent context mechanism, did not work flawlessly between multiple AST's.
	 *	The lazy-build mechanism loses its mapping when switching AST's and does not rebuild correctly.
	 *	Custom parent tracking, embedded in the traversal mechanism ensures alternative access to parent context.
	 *	@param S Statement that is pushed for tracking traversal heritage.
	 */
	bool TraverseStmt(Stmt *S) {
        parentStack.push_back(S);
        bool result = RecursiveASTVisitor<ContextGatherer>::TraverseStmt(S);
        parentStack.pop_back();
        return result;
	}
	
	/**
	 *	We expressly handle CallExpr visits, because only these can be potential markers.
	 *	From there we can derive the markee, if any.
	 *	@param CE CallExpr to process.
	 */
	bool VisitCallExpr(CallExpr *CE){
		indexProcessor(CE); 
		return true;
	}

	/**
	 *	Statement recorder decider function, which selects the appropriate recorder function for given AST-node.
	 *	Currently each function returns a bool, which allows for further action if necessary.
	 *	
	 *	NOTICE: bool feedback from recorder functions is currently unused.
	 *
	 *	@param node		Statement node to record
	 *	@param findings	Ledger where results get recorded
	 */
	void evalSRDeciderP(Stmt *node, json *findings){
		switch(node->getStmtClass()){
			default: 						generalStmtRecorder(node, findings); 	break;
//			case Stmt::CaseStmtClass:		caseStmtRecorder(node, findings);		break;
			case Stmt::DeclStmtClass: 		declStmtRecorder(node, findings); 		break;
			case Stmt::RecoveryExprClass: 	recoveryExprRecorder(node, findings); 	break;
			case Stmt::CallExprClass:		callExprRecorder(node, findings); 		break;
			case Stmt::ForStmtClass:		forStmtRecorder(node, findings); 		break;
			case Stmt::WhileStmtClass:		whileStmtRecorder(node,findings); 		break;
			case Stmt::DoStmtClass:			doStmtRecorder(node, findings); 		break;
			case Stmt::IfStmtClass:			ifStmtRecorder(node,findings); 			break;
			case Stmt::BinaryOperatorClass:	binaryOperatorRecorder(node,findings); 	break;
			case Stmt::SwitchStmtClass:		switchStmtRecorder(node,findings); 		break;
//			case Stmt::IntegerLiteralClass: break;
		}
	}
	
	/* 		Statement Recorders - Non-Expr		*/
	
	/**
	 *	Unhandled statements get generic recordings, could be used for diagnostics and surveying unhandled cases.
	 */
	bool generalStmtRecorder(Stmt *node, json *findings){
		(*findings)["properties"]["has_markee"] = true;

//		if (!isa<Expr>(node)) {
//			auto cast_node = llvm::cast<Expr>(node);
//			(*findings)["properties"]["contains_errors"] = cast_node->containsErrors();
//		}
		return false;
	}
	
	bool declStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<DeclStmt>(node);
		(*findings)["properties"]["has_markee"] = true;
		(*findings)["properties"]["single_decl"] = cast_node->isSingleDecl();
		(*findings)["properties"]["has_=_init"] = usesEqualsInitializer(cast_node);
		
		auto decls = cast_node->decls();
		for(auto uncast_decl: decls){
//			(*findings)["properties"]["decl_types"].push_back(uncast_decl->getKind());

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
				RD->isStruct();
				recordQTypeInfo(selectedAST->getTypeDeclType(RD), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        case clang::Decl::Enum: {
	            auto *ED = cast<clang::EnumDecl>(uncast_decl);
				recordQTypeInfo(selectedAST->getTypeDeclType(ED), (*findings)["properties"]["init_types"]);
	            break;
	        }
	        default:
	            // TODO: report unhandled case to diagnostics
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

	// Voor FunctionDecl aparte entries maken op basis van CallExpr.
	// We kunnen altijd de generieke en callexpr metrieken opnemen, behalve als er geen markee is.
	bool callExprRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<CallExpr>(node);
		string name = getCallExprName(cast_node, *selectedAST); // callee is not always available

		(*findings)["properties"]["name"] = name;
		(*findings)["properties"]["has_callee"] = false;

		if(name.find(markerFormat["name"].get<string>()) != string::npos){ // name contains marker substring, so markee is lost.
   			(*findings)["properties"]["has_markee"] = false;
			return false;
   		} else {
			(*findings)["properties"]["has_markee"] = true;
		}

		// als een marker wordt bekeken, pseudo of niet, dan is dit altijd een functiondecl in clang-ast
		// afwezigheid van de functiondecl is dus een indicator voor de aanwezigheid van een legitieme markee, naast de naamcheck
		auto ceDCCallee = cast_node->getDirectCallee();			
		auto ceDRECallee= cast_node->getCallee()->IgnoreImpCasts();		// if direct callee is not available we try the DeclRefExpr for unknown templated code.

		if(ceDCCallee){			
			// generic metrics
			(*findings)["properties"]["has_callee"] = true;
			
			// callee metrics
			(*findings)["properties"]["is_variadic"] = ceDCCallee->isVariadic();
			(*findings)["properties"]["num_params"] = ceDCCallee->getNumParams();
			(*findings)["properties"]["params"] = json::array();
			for (unsigned i = 0; i < ceDCCallee->getNumParams(); ++i) {
				recordQTypeInfo(ceDCCallee->getParamDecl(i)->getType(), (*findings)["properties"]["param_types"]);
	        }

		} else if(ceDRECallee){
//			cout << "Handled extra cases: "<< ceDRECallee->getStmtClassName() << "\n";
			if (const auto *declRef = llvm::dyn_cast<clang::DeclRefExpr>(ceDRECallee)) {
				if(const auto *funcDecl = llvm::dyn_cast_or_null<clang::FunctionDecl>(declRef->getDecl())){
					// generic metrics
					(*findings)["properties"]["has_callee"] = true;
					
					// callee metrics
					(*findings)["properties"]["is_variadic"] = funcDecl->isVariadic();
					(*findings)["properties"]["num_params"] = funcDecl->getNumParams();
					(*findings)["properties"]["params"] = json::array();
					for (unsigned i = 0; i < funcDecl->getNumParams(); ++i) {
						recordQTypeInfo(funcDecl->getParamDecl(i)->getType(), (*findings)["properties"]["param_types"]);
					}
				} 
			} 
		}
		
		// callexpr metrics
		(*findings)["properties"]["return_type"] = cast_node->getType().getAsString();
		(*findings)["properties"]["has_side_effects"] = cast_node->HasSideEffects(*selectedAST);

		(*findings)["properties"]["num_args"] = cast_node->getNumArgs();
		for (unsigned i = 0; i < cast_node->getNumArgs(); ++i) {
			recordQTypeInfo(cast_node->getArg(i)->getType(), (*findings)["properties"]["arg_types"]);
		}
		return false;
	}

	bool caseStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<CaseStmt>(node);
		(*findings)["properties"]["has_markee"] = true;
		
		Expr *LHS = cast_node->getLHS();
		Expr *RHS = cast_node->getRHS();
		
		(*findings)["properties"]["sub_stmt_class"] = cast_node->getSubStmt()->getStmtClassName();
		(*findings)["properties"]["lhs_is_value_dependent"] = LHS->isValueDependent();
		(*findings)["properties"]["is_gnu_range"] = cast_node->caseStmtIsGNURange();
		if(cast_node->caseStmtIsGNURange()){
			(*findings)["properties"]["rhs_is_value_dependent"] = RHS->isValueDependent();
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
			(*findings)["properties"]["cond_stmt_class"].push_back(conditionExpr->getStmtClassName());
//			(*findings)["properties"]["cond_expr_has_errors"] = conditionExpr->containsErrors();
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
			(*findings)["properties"]["cond_stmt_class"] = json::array();
		}		
		return false;
	}
	
	/**
	 * Some properties are only available for cpp, like condition variables.
	 * For assessment of C, metrics for these can be ignored. 
	 */
	bool ifStmtRecorder(Stmt *node, json *findings){
		auto cast_node = llvm::cast<IfStmt>(node);
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
			recordQTypeInfo(conditionExpr->getType(), (*findings)["properties"]["cond_expr_types"]);
		} else {
			(*findings)["properties"]["has_cond_expr"] = false;
			// report to diagnostics, shouldnt happen for C code.
		}
		
		// TODO: check for compound or dangling stmt status

		auto elseStmt = cast_node->getElse();
		if(elseStmt){
			(*findings)["properties"]["has_else"] = true;
			// maybe do more
		} else {
			(*findings)["properties"]["has_else"] = false;
		}
		
		// TODO: Continued branches check?
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

	// TODO: entries should give diagnostic effects, decls are still WIP.
	
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

