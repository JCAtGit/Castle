#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <vector>
#include <string>
#include "../../../../../include/helper.hpp"
#include "../preprocessor-mechanism/pp-processor-stmt.hpp"
#include "../preprocessor-mechanism/pp-processor-decl.hpp"
#include "predicates-stmt.hpp"
#include "predicates-decl.hpp"

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
		int markerID = 0;
		string markerName;
		string markerKeyword;
		string markerType;
		string markerSignature;
		string markerEndString = "; \n";
		string headerInclusion;
		string definitionFilename;
		SourceManager& SM;
		
		virtual string synthesizeMarkerDeclarationString(){
			return markerKeyword + markerType + markerName + markerSignature + markerEndString;
		}
		
		virtual string synthesizeCallMarkerString(){
			string markerSigInstance = "(" + std::to_string(markerID) + ")";
			(markerID)++;
			return markerName + markerSigInstance + markerEndString;
		}
		
		string synthesizeMarkerDependencyString(filesystem::path inputPath, filesystem::path inputDir){
			string relativeTrail = getDescend2BaseDirString(rel_outputPath(inputDir, inputPath));
			return headerInclusion + "\""+ relativeTrail + definitionFilename + ".h" + "\"\n";
		}
		
	public:
		CodeMarkerVisitor(
			ASTContext& context, 	Rewriter* rewriter, 	syntax::TokenBuffer tokenBuffer, 		vector<RecordedPPConstruct>& ppEvents, 	json markerFormatCfg):
			context(context),		rewriter(rewriter),		tokenBuffer(std::move(tokenBuffer)), 	ppEvents(ppEvents), 					markerFormatCfg(markerFormatCfg),
			SM(context.getSourceManager()){
				markerName 			= 	markerFormatCfg["name"].get<string>();
				markerKeyword 		= 	markerFormatCfg["keyword"].get<string>();
				markerType 			= 	markerFormatCfg["functionType"].get<string>();
				markerSignature 	= 	markerFormatCfg["paramSignature"].get<string>();
				headerInclusion 	=   markerFormatCfg["headerInclusionDirective"].get<string>();
				definitionFilename 	=	markerFormatCfg["definitionFileName"].get<string>();
			}
	
			void insertMarkerDependency(filesystem::path inputPath, filesystem::path inputDir){
				SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
				StringRef filePath = SM.getFilename(startLoc);
				rewriter->InsertTextBefore(startLoc,synthesizeMarkerDependencyString(inputPath, inputDir));
			}
			
			void commitToSingleFile(filesystem::path inputPath, filesystem::path outputPath){
				// returns empty if path is on same dir.
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));	
				if(!filesystem::exists(outputPath)){
					filesystem::create_directories(outputPath.parent_path());
				}
				// write to file
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(outputPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {
					llvm::errs() << "Error opening output file: " << EC.message()<< "\n";
					return;
				}
				rewriter->getEditBuffer(rewriter->getSourceMgr().getMainFileID()).write(OutFile);
				OutFile.close();	
			}
			
			void commitToQueueFile(filesystem::path inputDir, filesystem::path outputDir){
				cout << "Start commit phase" << "\n";
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));
				filesystem::path outPath = outputDir / rel_outputPath(inputDir, FilePath.str());
				if (outPath.empty() || outPath == "." || outPath == "./") {
				    outPath = FilePath.str();
				}
				llvm::outs() << "Committing changes from: " << FilePath << " -> " << outPath << "\n";
				if(!filesystem::exists(outPath)){
					filesystem::create_directories(outPath.parent_path());
				}
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(outPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {
					llvm::errs() << "Error opening output file: " << EC.message()<< "\n";
					return;
				}
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
					if (Parent.getSourceRange().getBegin().isMacroID()) {return true;}
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
						    processFunctionDecl(cast<FunctionDecl>(D)); break;
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
						default: break;
						}
					}
				return true;
		}
		
		//--Declarations--
		void processDecl(Decl* D) {}
		
		// TODO: Mark start and end of function and account for macros?
		void processFunctionDecl(FunctionDecl* FD){
			
		}
			
		//--Statements--
		void processDeclStmt(DeclStmt* DS) {
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
			
		void processForStmt(ForStmt* FS) {
			rewriter->InsertText(
			FS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		void processWhileStmt(WhileStmt* WS){
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
		void processSwitchStmt(SwitchStmt* SS){
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
		void processCaseStmt(CaseStmt* CS){
			rewriter->InsertText(
			CS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		void processCallExpr(CallExpr* CE){
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
		
		void processGotoStmt(GotoStmt* GS){
			rewriter->InsertText(
			GS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
};
