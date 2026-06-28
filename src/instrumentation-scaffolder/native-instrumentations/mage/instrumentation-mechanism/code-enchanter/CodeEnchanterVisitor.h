#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"

#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include "clang/Sema/Sema.h"
#include "clang/AST/Attr.h"

#include <filesystem>
#include <vector>
#include <string>
#include <helper.hpp>
#include "../preprocessor-mechanism/pp-processor-stmt.hpp"
#include "../preprocessor-mechanism/pp-processor-decl.hpp"
#include "../preprocessor-mechanism/PPHelper.hpp"
#include "predicates-stmt.hpp"
#include "predicates-decl.hpp"

using namespace clang;
using namespace std;
using namespace nlohmann;

class CodeEnchanterVisitor: public RecursiveASTVisitor<CodeEnchanterVisitor> {
	private:
		Rewriter& rewriter;
		syntax::TokenBuffer tokenBuffer;
		vector<RecordedPPConstruct> recordedPPEventsEvents;
		CompilerInstance& CI;
		
		SourceManager& SM;
		ASTContext& context;

		json markerFormatCfg;
		int lastStart;
		int& idCounter;
		bool& nameEncodedMarker;
		bool& clangFormatMode;
		
		string markerName;
		string markerKeyword;
		string markerType;
		string markerSignature;
		string markerEndString = "; \n";
		string headerInclusion;
		string definitionFilename;

		string synthesizeCallMarkerString(){
			string markerCallString = markerName;
			if(nameEncodedMarker){
				markerCallString += std::to_string(idCounter);
			}
			markerCallString +=  "(" + std::to_string(idCounter) + ")" + markerEndString;
			idCounter++;
			return markerCallString;
		}
		
		string synthesizeInclusionDirective(filesystem::path inputPath, filesystem::path inputDir){
			string relativeTrail = getDescend2BaseDirString(rel_outputPath(inputDir, inputPath));
			return headerInclusion + "\""+ relativeTrail + definitionFilename + ".h" + "\"\n";
		}
		
		string synthesizePragmaDirectives(){
			return markerFormatCfg["noOptPragma"];
		}
		
	public:
		CodeEnchanterVisitor(
			CompilerInstance &ci, 
			Rewriter &rewriter, 
			syntax::TokenBuffer tokenBuffer, 
			vector<RecordedPPConstruct>& recordedPPEventsEvents, 
			json &markerFormatCfg, 
			int &idCounter,
			bool &nameEncodedMarkers,
			bool &clangFormatMode
		):
			CI(ci), 
			rewriter(rewriter),	
			tokenBuffer(std::move(tokenBuffer)), 
			recordedPPEventsEvents(recordedPPEventsEvents), 
			markerFormatCfg(markerFormatCfg), 
			idCounter(idCounter),
			nameEncodedMarker(nameEncodedMarkers),
			clangFormatMode(clangFormatMode),
			
			context(ci.getASTContext()), SM(ci.getASTContext().getSourceManager()){
				markerName 			= 	markerFormatCfg["name"].get<string>();
				markerKeyword 		= 	markerFormatCfg["keyword"].get<string>();
				markerType 			= 	markerFormatCfg["functionType"].get<string>();
				markerSignature 	= 	markerFormatCfg["paramSignature"].get<string>();
				headerInclusion 	=   markerFormatCfg["headerInclusionDirective"].get<string>();
				definitionFilename 	=	markerFormatCfg["definitionFileName"].get<string>();
				lastStart = idCounter;
			}
	
			/**
			 *  Include and other directives like:
	 		 *		#if defined(__clang__)
			 *	    	#pragma clang optimize off
			 *	 	#elif defined(__GNUC__)
			 *	     	#pragma GCC push_options
			 *	     	#pragma GCC optimize ("O0")
			 *	 	#endif
			 */
			void insertPreprocessorDirectives(filesystem::path inputPath, filesystem::path inputDir){
				SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
				StringRef filePath = SM.getFilename(startLoc);
				rewriter.InsertTextBefore(startLoc, synthesizeInclusionDirective(inputPath, inputDir));
			}
			
			void insertNameEncodedMarkerDefinitions(){
				SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
				string localDefinition;
				for(; lastStart < idCounter; lastStart++){
					localDefinition += 
					markerFormatCfg["noOptAttributes"].get<string>()	+ " " +
					markerFormatCfg["functionType"].get<string>() 		+ " " +
					markerFormatCfg["name"].get<string>() 				+
					std::to_string(lastStart)						+
					markerFormatCfg["paramSignature"].get<string>()		+ "{" +
					markerFormatCfg["definition"].get<string>()			+ "};"+
					"\n"; 
				}
				rewriter.InsertTextBefore(startLoc, localDefinition);
			}
			
			void commitToSingleFile(filesystem::path inputPath, filesystem::path outputPath, filesystem::path outputDir){
				// returns empty if path is on same dir.
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));	
				if(!filesystem::exists(outputPath)){
					filesystem::create_directories(outputPath.parent_path());
				}
				// write to file
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(outputPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {
					llvm::errs() << "Error 1 opening output file: " << EC.message() << "\n";
					return;
				}
				rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
				OutFile.close();	
			}
			
			void commitToQueueFile(filesystem::path inputDir, filesystem::path outputDir){
//				cout << "Start commit phase" << "\n";
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));
				filesystem::path outPath = outputDir / rel_outputPath(inputDir, FilePath.str());
//				if (outPath.empty() || outPath == "." || outPath == "./" || ".") {
//				    outPath = FilePath.str();
//				} 
//				llvm::outs() << "Commit changes from: " << FilePath << " -> " << outPath << "\n";
				if(!filesystem::exists(outPath)){
					filesystem::create_directories(outPath.parent_path());
				}
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(outPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {
					llvm::errs() << "Error 2 opening output file: " << EC.message()<< "\n";
					return;
				}
				rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
				OutFile.close();	
			}
			
		bool isStmtLocationSafe(Stmt *S, const SourceManager &SM, const LangOptions &LangOpts) {
		    if (!S) return false;
		    SourceLocation Begin = S->getBeginLoc();
		    SourceLocation End = S->getEndLoc();
		    if (Begin.isInvalid() || End.isInvalid()) return false;
		    // If either location is a macro ID, the range might be unsafe.
		    if (Begin.isMacroID() || End.isMacroID()) return false;

		    // Get the full source range of the statement as a char range.
		    CharSourceRange StmtRange = CharSourceRange::getTokenRange(Begin, End);
		    CharSourceRange FileRange = Lexer::makeFileCharRange(StmtRange, SM, LangOpts);
		    if (FileRange.isInvalid()) return false;

		    // Ensure the range is in a file (not a macro expansion buffer).
		    return FileRange.getBegin().isFileID();
		}	
		
		bool VisitStmt(Stmt *S) {
			if (SM.isInMainFile(S->getBeginLoc())){				
				//-- Start marker safety checks
				//-- Surrounding ASTNodes check
//				if(!stmtHasValidPrevSibling(S, context)){return;}
				if(!stmtHasValidChildCriteria(S)){return true;}
				//-- Section: Preprocessor directives check
				auto detectedEvents = detectPPConstructsAroundStatement(S, tokenBuffer, SM, recordedPPEventsEvents);
				if(!isPPSafeStmtMarking(detectedEvents)){return true;}
				if (!isStmtLocationSafe(S, SM, CI.getLangOpts())) return true;

				//-- Clang Macro expansion id check.
				DynTypedNodeList Parents = context.getParents(*S); // TODO: migrate to parentStack method?
				for (const auto &Parent : Parents) {
					if (Parent.getSourceRange().getBegin().isMacroID()) {return true;}
				}
				
				if(!clangFormatMode){
					if(Parents.empty()) {
						return true;
					} else if(!Parents[0].get<CompoundStmt>()){
						return true;
					}
				}
				
				if(S->getBeginLoc().isMacroID() || S->getBeginLoc().isInvalid()){return true;}
				//-- End safety checks;
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

		bool VisitDecl(Decl *D){
			if (SM.isInMainFile(D->getLocation())){
				//-- Section: Preprocessor directives check
				auto detectedEvents = detectPPConstructsAroundDeclaration(D, tokenBuffer, SM, recordedPPEventsEvents);
				if(!isPPSafeStmtMarking(detectedEvents)){return true;}

				// Clang Macro expansion id check.
				DynTypedNodeList Parents = context.getParents(*D);
				for (const auto &Parent : Parents) {
					if (Parent.getSourceRange().getBegin().isMacroID()) {
						return true;
					}
				}
				if(D->getBeginLoc().isMacroID()){return true;}
				//-- End safety checks;
				
				switch (D->getKind()) {
					case Decl::Function:
					    processFunctionDecl(cast<FunctionDecl>(D)); break;
					default: break;
					}
				}
			return true;
		}
		
		// --Declarations--
		void processDecl(Decl* D) {}
		
		/**
		 * TODO: Safe inline removals:
		 * https://stackoverflow.com/questions/79589301/will-removal-of-all-inline-keywords-from-my-code-leave-my-c-program-semantical
		 * 
		 * Useful info:
		 * https://clang.llvm.org/docs/AttributeReference.html#optnone
		 */
		void processFunctionDecl(FunctionDecl* FD){
			if(FD->hasAttr<AlwaysInlineAttr>() || FD->hasAttr<MinSizeAttr>() || FD->hasAttr<OptimizeNoneAttr>()){return;}
			addOrCreateGNUOptnone(FD, rewriter, SM, CI.getLangOpts());			
		}
			
		// --Statements--
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
			rewriter.InsertText(
			begin,
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
			
		void processForStmt(ForStmt* FS) {
			rewriter.InsertText(
			FS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		void processWhileStmt(WhileStmt* WS){
			rewriter.InsertText(
			WS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		virtual void processIfStmt(IfStmt* IS) {
			rewriter.InsertText(
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
			rewriter.InsertText(
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
			rewriter.InsertText(
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
					rewriter.InsertText(
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
			rewriter.InsertText(GS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
};
