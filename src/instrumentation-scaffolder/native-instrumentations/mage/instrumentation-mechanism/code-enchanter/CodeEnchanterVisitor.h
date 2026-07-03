#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

#include <filesystem>
#include <string>
#include "paths.hpp"
#include "predicates-stmt.hpp"
#include "../preprocessor/MacroExpansionRecorder.hpp"
#include "../preprocessor/attributes.hpp"
using namespace clang;
using namespace std;
using namespace nlohmann;

/**
 *	Visitor class for source code. 
 *	Implements: Marker, Attribute Graft Enhancements (MAGE)
 */
class CodeEnchanterVisitor: public RecursiveASTVisitor<CodeEnchanterVisitor> {
	private:
		Rewriter& 				rewriter;
		MacroExpansionRecorder  *macroRecorder = nullptr;
  		CompilerInstance& 		CI;
		SourceManager& 			SM;
		ASTContext& 			context;

		json 					markerFormatCfg;
		int 					lastStart,
								&idCounter;
		bool 					&nameEncodedMarker, 
								&clangFormatMode;
		
		string 					markerName,
								markerKeyword,
		 						markerType,
		 						markerSignature,
		 						markerEndString = "; \n",
		 						headerInclusion,
								definitionFilename;

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
			CompilerInstance 		&ci, 
			Rewriter 				&rewriter, 
			MacroExpansionRecorder  *macroRecorder,
			json 					&markerFormatCfg, 
			int 					&idCounter,
			bool 					&nameEncodedMarkers,
			bool 					&clangFormatMode
		):
			CI(ci), 
			rewriter(rewriter),	
			macroRecorder(macroRecorder),
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
			
			/**
			 * 	Sometimes decompilers do not recover the index of a marker, however they can still succeed in preserving the name (given debug info).
			 *	Name encoded markers are failsafe mechanism to ensure marker mappability.
			 */
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
				if(!filesystem::exists(outputPath)){filesystem::create_directories(outputPath.parent_path());}
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(outputPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {llvm::errs() << "commitToSingleFile: error opening output file: " << EC.message() << "\n"; return;}
				rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
				OutFile.close();	
			}
			
			void commitToQueueFile(filesystem::path inputDir, filesystem::path outputDir){
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));
				filesystem::path outPath = outputDir / rel_outputPath(inputDir, FilePath.str());
				if(!filesystem::exists(outPath)){filesystem::create_directories(outPath.parent_path());}
				std::error_code EC;
				llvm::raw_fd_ostream OutFile(outPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {llvm::errs() << "commitToQueueFile: error opening output file: " << EC.message()<< "\n"; return;}
				rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
				OutFile.close();	
			}
		
		bool VisitStmt(Stmt *S) {
			if (SM.isInMainFile(S->getBeginLoc())){				
				//-- Start marker safety checks
				if(!stmtHasValidChildCriteria(S)){return true;}
//				if (macroRecorder->hasOverlap(S->getSourceRange())) {
//					return true;
//				}
				
				unsigned stmtStart = SM.getFileOffset(SM.getSpellingLoc(S->getBeginLoc()));
				if (macroRecorder->isOffsetInsideMacro(stmtStart)) {
				    return true;   // cannot inject before – start is inside a macro
				}

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
					case Stmt::IfStmtClass: 	processIfStmt(cast<IfStmt>(S)); 		break;
					case Stmt::WhileStmtClass: 	processWhileStmt(cast<WhileStmt>(S)); 	break;
					case Stmt::DeclStmtClass: 	processDeclStmt(cast<DeclStmt>(S)); 	break;
					case Stmt::ForStmtClass: 	processForStmt(cast<ForStmt>(S)); 		break;
					case Stmt::CallExprClass: 	processCallExpr(cast<CallExpr>(S)); 	break;
					case Stmt::SwitchStmtClass: processSwitchStmt(cast<SwitchStmt>(S));break;
					case Stmt::CaseStmtClass: 	processCaseStmt(cast<CaseStmt>(S)); 	break;
					case Stmt::GotoStmtClass: 	processGotoStmt(cast<GotoStmt>(S)); 	break;
					default: break;
				}
			}
			return true;
		}

		bool VisitDecl(Decl *D){
			if (SM.isInMainFile(D->getLocation())){
				//-- Section: Preprocessor directives check
//				if (macroRecorder->hasOverlap(D->getSourceRange())) {
//					return true;
//				}

				unsigned stmtStart = SM.getFileOffset(SM.getSpellingLoc(D->getBeginLoc()));
				if (macroRecorder->isOffsetInsideMacro(stmtStart)) {
				    return true;   // cannot inject before – start is inside a macro
				}
				
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
		
//------	Declarations	------
		void processDecl(Decl* D) {}
		
		/**
		 * TODO: Safe inline removals:
		 * https://stackoverflow.com/questions/79589301/will-removal-of-all-inline-keywords-from-my-code-leave-my-c-program-semantical
		 * 
		 * Useful info:
		 * https://clang.llvm.org/docs/AttributeReference.html#optnone
		 */
		void processFunctionDecl(FunctionDecl* FD){
			unsigned fdStart = SM.getFileOffset(SM.getSpellingLoc(FD->getBeginLoc()));
			if (macroRecorder->isOffsetInsideMacro(fdStart)) {
			    return;   // cannot inject before – start is inside a macro
			}
//			SourceLocation Loc = FD->getBeginLoc();
//			if (context.getSourceManager().isMacroBodyExpansion(Loc) || 
//			    context.getSourceManager().isMacroArgExpansion(Loc)) {
//			    return; // Do not textually modify functions declared via macro trickery
//			}
			
			addOrCreateGNUOptnone(FD, rewriter, SM, CI.getLangOpts());			
		}
		
//------	Statements		------
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
		 * 	TODO: 
		 *		Mark cases? or simply retrieve case info from within the switch stmt.
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
		 * These markers statements are unreachable, but the static analyzer does not signal any breaking errors.
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
