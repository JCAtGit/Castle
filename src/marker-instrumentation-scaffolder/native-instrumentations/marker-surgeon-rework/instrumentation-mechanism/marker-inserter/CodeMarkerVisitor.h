#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include "RecordingPPCallbacks.hpp"
#include <vector>
#include <string>
#include "../../preprocessor-mechanism/directives/directive-processor.hpp"
#include "markerPredicates.hpp"

using namespace clang;
using namespace std;
using namespace nlohmann;

class CodeMarkerVisitor: public RecursiveASTVisitor<CodeMarkerVisitor> {
	private:
		ASTContext& context;
		Rewriter* rewriter;
		syntax::TokenBuffer tokenBuffer;
		vector<RecordedPPConstruct> ppEvents;
		json markerFormatCfg;
		int markerID;
		string markerName;
		string markerKeyword;
		string markerType;
		string markerSignature;
		string markerDefinition;
		
		SourceManager& SM;

		virtual string synthesizeMarkerDeclarationString(){
			string markerString = markerKeyword + markerType + markerName + markerSignature + markerEndString;
			return markerString;
		}
		
		virtual string synthesizeCallMarkerString(){
			string markerSigInstance = "(" + std::to_string(markerID) + ")";
			string markerString = markerName + markerSigInstance + markerEndString;
			(markerID)++;
			return markerString;
		}

		virtual string synthesizeMarkerDefinitionString(){
			string markerString = markerType + markerName + markerSignature + markerDefinition + markerEndString;
			return markerString;
		}
		
		//			TODO: Change to extern decl.
		virtual string synthesizeMarkerDependencyString(string fileIn, struct config *cfg){
	//			string headerInclusionString = headerInclusionDirective;
	//			string relativeTrail = getDescend2BaseDirString(rel_outputPath(cfg->inputDir, fs::path(fileIn)));
	//			return headerInclusionString + "\""+ relativeTrail + cfg->markerDefinitionFileName + ".h" + "\"\n";
				return markerKeyword + markerType + markerName + markerSignature +";\n";
			}
			
		
		/**
		 * 	This file will be used to link separately, so that an optimization barrier can be created.
		 *  The created file will be placed at the output root.
		 *  Check the preview folder for an example output.
		 TODO: Change constant extraction mechanism
		 */
		static void createMarkerDefinitionHeaderFile(struct config& cfg){
			llvm::outs() << "Start marker header file creation." << "\n";
			string content = 
				inclusionDirectiveIFNDEF + " " + inclusionDirectiveInclGuard + "\n" +
				inclusionDirectiveDEF + " " + inclusionDirectiveInclGuard + "\n" +
				cfg.markerFunctionSignatureString + '\n' + 
				inclusionDirectiveENDIF;
						
			// Open the file; if it exists, it will be overwritten.
		    error_code EC;
		    llvm::raw_fd_ostream out((cfg.outputDir / (cfg.markerDefinitionFileName + ".h")).string(), EC, llvm::sys::fs::OF_None);
		    if (EC) {llvm::errs() << "Error opening file: " << EC.message() << "\n";}

		    // Write the string to the file and optionally flush (the destructor will close the file automatically).
		    out << content;
		    out.flush();
			llvm::outs() << "Marker header file written. \n";
		}
		
		static void createMarkerDefinitionSourceFile(struct config& cfg){
			llvm::outs() << "Start marker source file creation." << "\n";
			string content = 
				headerInclusionDirective + "\"" + (cfg.markerDefinitionFileName + ".h") + "\"\n" +
				cfg.markerDefinitionString;
			
			// Open the file; if it exists, it will be overwritten.
		    error_code EC;
		    llvm::raw_fd_ostream out((cfg.outputDir / (cfg.markerDefinitionFileName + ".c")).string(), EC, llvm::sys::fs::OF_None);
		    if (EC) {llvm::errs() << "Error opening file: " << EC.message() << "\n";}

		    // Write the string to the file and optionally flush (the destructor will close the file automatically).
		    out << content;
		    out.flush();
			llvm::outs() << "Marker source file written. \n";
		}
		
		static void createDependencyFiles(struct config& cfg) {
			createMarkerDefinitionHeaderFile(cfg);
			createMarkerDefinitionSourceFile(cfg);
		}

		
	//--Declarations--
		
		virtual void processDecl(Decl* D) {}
		
		// TODO: Mark start and end of function and account for macros?
		virtual void processFunctionDecl(FunctionDecl* FD){}
		
	//--Statements--

		virtual void processDeclStmt(DeclStmt* DS) {
			DynTypedNodeList Parents = context->getParents(*DS);
			for (const auto &Parent : Parents) {
				if (const auto *ParentStmt = Parent.get<Stmt>()) {
					// If parent node is one of the following, then we skip the marking.
					// Otherwise we mark the parameters within these constructs, leading to invalid syntax.
					if(isa<ForStmt>(ParentStmt)){return;} 
					else if (isa<IfStmt>(ParentStmt)) {return;} 
					else if (isa<SwitchStmt>(ParentStmt)) {return;} 
					else if (isa<AttributedStmt>(ParentStmt)) {return;}
				}
			}
			
			SourceLocation begin = DS->getBeginLoc();
			R->InsertText(
			begin,
			synthesizeCallMarkerString(), 
			false, 
			true
			);
			
	//		llvm::outs() << "DeclStmt found at " << DS->getSourceRange().getBegin().printToString(context->getSourceManager()) << "\n";
	//		if (auto macroName = getMacroNameIfStmtStartsWithMacro(DS, *tokenBuffer, context->getSourceManager(), context->getLangOpts())) {
	//		        llvm::outs() << "DeclStmt originates from macro: " << *macroName << "\n";
	//		}
		}
		
		virtual void processForStmt(ForStmt* FS) {
			R->InsertText(
			FS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		virtual void processWhileStmt(WhileStmt* WS){
			R->InsertText(
			WS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		virtual void processIfStmt(IfStmt* IS) {
			R->InsertText(
			IS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		/**
		 * TODO: Mark cases of niet?
		 */
		virtual void processSwitchStmt(SwitchStmt* SS){
			R->InsertText(
			SS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		/**
		 * Deze marker statements zijn unreachable, maar de static analyzer geeft verder geen meldingen hierover.
		 */
		virtual void processCaseStmt(CaseStmt* CS){
			R->InsertText(
			CS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		virtual void processCallExpr(CallExpr* CE){
			DynTypedNodeList Parents = context->getParents(*CE);
			for (const auto &Parent : Parents) {
				if (const auto *ParentNode = Parent.get<CompoundStmt>()) {
					R->InsertText(
						CE->getBeginLoc(),
						synthesizeCallMarkerString(), 
						false, 
						true
					);
					return;
				}
			}
		}
		
		virtual void processGotoStmt(GotoStmt* GS){
			R->InsertText(
			GS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
	public:
		CodeMarkerVisitor(
			ASTContext& context, 	Rewriter* rewriter, 	syntax::TokenBuffer tokenBuffer, 		vector<RecordedPPConstruct>& ppEvents, 	json markerFormatCfg):
			context(context),		rewriter(rewriter),		tokenBuffer(std::move(tokenBuffer)), 	ppEvents(ppEvents), 					markerFormatCfg(markerFormatCfg),
			
			SM(context.getSourceManager()){
				
			}
	
		bool VisitStmt(Stmt *S) {
			if (SM.isInMainFile(S->getBeginLoc())){
				//-- Start marker safety checks
				//-- Surrounding ASTNodes check
//				if(!stmtHasValidPrevSibling(S, context)){return;}
				if(!stmtHasValidChildCriteria(S)){return true;}
				//-- Section: Preprocessor directives check
				auto ppStmtEvents = detectPPConstructsAroundStatement(S, icp->getTokenBuffer(), SM, ppEvents);
				if(!isPPSafeStmtMarking(ppStmtEvents)){return true;}
				//--  Clang Macro expansion id check.
				DynTypedNodeList Parents = context.getParents(*S);
				for (const auto &Parent : Parents) {
					if (Parent.getSourceRange().getBegin().isMacroID()) {
						return true;
					}
				}
				if(S->getBeginLoc().isMacroID()){return true;}
				//-- End marker safety checks;
				switch (S->getStmtClass()) {
					case Stmt::IfStmtClass: icp->processIfStmt(cast<IfStmt>(S)); break;
					case Stmt::WhileStmtClass: icp->processWhileStmt(cast<WhileStmt>(S)); break;
					case Stmt::DeclStmtClass: icp->processDeclStmt(cast<DeclStmt>(S)); break;
					case Stmt::ForStmtClass: icp->processForStmt(cast<ForStmt>(S)); break;
					case Stmt::CallExprClass: icp->processCallExpr(cast<CallExpr>(S)); break;
					case Stmt::SwitchStmtClass: icp->processSwitchStmt(cast<SwitchStmt>(S)); break;
					case Stmt::CaseStmtClass: icp->processCaseStmt(cast<CaseStmt>(S)); break;
					case Stmt::GotoStmtClass: icp->processGotoStmt(cast<GotoStmt>(S)); break;
					default: break;
				}
			}
			return true;
		}

		bool visitDecl(Decl *D){
		// TODO: Implement when necessary, not in use currently.
					if (SM.isInMainFile(D->getLocation())){
						//-- Section: Preprocessor directives check
		//				auto ppStmtEvents = detectPPConstructsAroundStatement(S, CP->getTokenBuffer(), SM, ppEvents);
		//				if(!isPPSafeStmtMarking(ppStmtEvents)){return;}
		//				// Clang Macro expansion id check.
		//				DynTypedNodeList Parents = context.getParents(*S);
		//				for (const auto &Parent : Parents) {
		//					if (Parent.getSourceRange().getBegin().isMacroID()) {
		//						return;
		//					}
		//				}
		//				if(S->getBeginLoc().isMacroID()){return;}
						//-- ;	
						switch (D->getKind()) {
								case Decl::Function:
		//						    CP->processFunctionDecl(cast<FunctionDecl>(D));
								    break;
								case Decl::Var:
		//						    ProcessVariable(cast<VarDecl>(D));
								    break;
								case Decl::Record: break;
								case Decl::CXXRecord: //subclass of RecordDecl
		//						    ProcessRecord(cast<RecordDecl>(D));
								    break;
								case Decl::Enum:
		//						    ProcessEnum(cast<EnumDecl>(D));
								    break;
								case Decl::Typedef:
		//						    ProcessTypedef(cast<TypedefDecl>(D));
								    break;
								default:
								    // Optionally handle other declaration kinds or do nothing
								    break;
								}
							}
						return true;
		}
};
