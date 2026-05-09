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
#include "../../../../../include/helper.hpp"

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
		string markerEndString = "; \n";

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
			return markerType + markerName + markerSignature + markerDefinition + markerEndString;
		}
		
		virtual string synthesizeMarkerDependencyString(){
			return markerKeyword + markerType + markerName + markerSignature +";\n";
		}
		
	public:
		CodeMarkerVisitor(
			ASTContext& context, 	Rewriter* rewriter, 	syntax::TokenBuffer tokenBuffer, 		vector<RecordedPPConstruct>& ppEvents, 	json markerFormatCfg):
			context(context),		rewriter(rewriter),		tokenBuffer(std::move(tokenBuffer)), 	ppEvents(ppEvents), 					markerFormatCfg(markerFormatCfg),
			SM(context.getSourceManager()){
				markerName = markerFormatCfg[""].get<string>();
				markerKeyword = markerFormatCfg[""].get<string>();
				markerType = markerFormatCfg[""].get<string>();
				markerSignature = markerFormatCfg[""].get<string>();
				markerDefinition = markerFormatCfg[""].get<string>();
			}
	
			// TODO: Generalize and change
			void insertMarkerDependency(){
				SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
				FileID mainFileID = SM.getMainFileID();
				StringRef filePath = SM.getFilename(startLoc);
				rewriter->InsertTextBefore(
					startLoc,
					synthesizeMarkerDependencyString()
				);
			}
			
			void commitToFile(string inputDir, string outputDir){
				llvm::outs() << "Start commit phase" << "\n";
				insertMarkerDependency();			

				// returns empty if path is on same dir.
				FileID MainFileID = SM.getMainFileID();
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(MainFileID));

				filesystem::path outPath = outputDir / rel_outputPath(inputDir, FilePath.str());
				if (outPath.empty() || outPath == "." || outPath == "./") {
				    outPath = FilePath.str();
				}
				llvm::outs() << "Committing changes from: " << FilePath << " -> " << outPath << "\n";
				// committing buffered changes to file
				if(!filesystem::exists(outPath.string())){
					filesystem::create_directories(outPath.parent_path());
				}
				// write to file and mutate with srcExt.
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(
					filesystem::path(outPath).replace_filename(outPath.stem().string() + outPath.extension().string()).string(),
					EC, 
					llvm::sys::fs::OF_Text
				);

				if (EC) {
					llvm::errs() << "Error opening output file: " << EC.message()<< "\n";
					return;
				}

				// Write buffered changes to file.
				rewriter->getEditBuffer(rewriter->getSourceMgr().getMainFileID()).write(OutFile);
				OutFile.close();	
			}
			
		bool VisitStmt(Stmt *S) {
			if (SM.isInMainFile(S->getBeginLoc())){
				//-- Start marker safety checks
				//-- Surrounding ASTNodes check
//				if(!stmtHasValidPrevSibling(S, context)){return;}
				if(!stmtHasValidChildCriteria(S)){return true;}
				//-- Section: Preprocessor directives check
				auto ppStmtEvents = detectPPConstructsAroundStatement(S, tokenBuffer, SM, ppEvents);
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
					case Stmt::IfStmtClass: processIfStmt(cast<IfStmt>(S)); break;
					case Stmt::WhileStmtClass: processWhileStmt(cast<WhileStmt>(S)); break;
					case Stmt::DeclStmtClass: processDeclStmt(cast<DeclStmt>(S)); break;
					case Stmt::ForStmtClass: processForStmt(cast<ForStmt>(S)); break;
					case Stmt::CallExprClass: processCallExpr(cast<CallExpr>(S)); break;
					case Stmt::SwitchStmtClass: processSwitchStmt(cast<SwitchStmt>(S)); break;
					case Stmt::CaseStmtClass: processCaseStmt(cast<CaseStmt>(S)); break;
					case Stmt::GotoStmtClass: processGotoStmt(cast<GotoStmt>(S)); break;
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
		
		//--Declarations--
				
			virtual void processDecl(Decl* D) {}
			
			// TODO: Mark start and end of function and account for macros?
			virtual void processFunctionDecl(FunctionDecl* FD){}
			
		//--Statements--

			virtual void processDeclStmt(DeclStmt* DS) {
				DynTypedNodeList Parents = context.getParents(*DS);
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
				rewriter->InsertText(
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
				rewriter->InsertText(
				FS->getBeginLoc(),
				synthesizeCallMarkerString(), 
				false, 
				true
				);
			}
			
			virtual void processWhileStmt(WhileStmt* WS){
				rewriter->InsertText(
				WS->getBeginLoc(),
				synthesizeCallMarkerString(), 
				false, 
				true
				);
			}
			
			virtual void processIfStmt(IfStmt* IS) {
				rewriter->InsertText(
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
				rewriter->InsertText(
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
				rewriter->InsertText(
				CS->getBeginLoc(),
				synthesizeCallMarkerString(), 
				false, 
				true
				);
			}
			
			virtual void processCallExpr(CallExpr* CE){
				DynTypedNodeList Parents = context.getParents(*CE);
				for (const auto &Parent : Parents) {
					if (const auto *ParentNode = Parent.get<CompoundStmt>()) {
						rewriter->InsertText(
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
				rewriter->InsertText(
				GS->getBeginLoc(),
				synthesizeCallMarkerString(), 
				false, 
				true
				);
			}
};
