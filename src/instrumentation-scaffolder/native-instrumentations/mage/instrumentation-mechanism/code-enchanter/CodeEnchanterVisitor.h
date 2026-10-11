#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
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
			string relativeTrail = getDescend2BaseDirString(filesystem::relative(inputPath, inputDir));
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
				markerType 			= 	markerFormatCfg["functionType"].get<string>();
				markerSignature 	= 	markerFormatCfg["paramSignature"].get<string>();
				headerInclusion 	=   markerFormatCfg["headerInclusionDirective"].get<string>();
				definitionFilename 	=	markerFormatCfg["definitionFileName"].get<string>();
				lastStart 			= 	idCounter;
			}
	
			/**
			 *  Include and other directives like:
	 		 *		#if defined(__clang__)
			 *	    	#pragma clang optimize off
			 *	 	#elif defined(__GNUC__)
			 *	     	#pragma GCC push_options
			 *	     	#pragma GCC optimize ("O0")
			 *	 	#endif
			 *	
			 *	But also determine the relative trailing for the file;
			 *	Examples:		
			 *		include "../../MageAlpha.h"
			 *		include "MageAlpha.h"
			 *		etc.
			 *
			 *	Inserts the preprocessor-directives and its inclusion into current processed file.
			 *	@param	inputPath	this path is used with inputDir to determine the relative path offset of the inclusion directive.
			 *	@param	inputDir	this dir is used with inputPath to determine the relative path offset of the inclusion directive.
			 */
			void insertPreprocessorDirectives(filesystem::path inputPath, filesystem::path inputDir){
				SourceLocation startLoc = SM.getLocForStartOfFile(SM.getMainFileID());
				StringRef filePath = SM.getFilename(startLoc);
				rewriter.InsertTextBefore(startLoc, synthesizeInclusionDirective(inputPath, inputDir));
			}
			
			/**
			 * 	Sometimes decompilers do not recover the index of a marker, however they can still succeed in preserving the name (given debug info).
			 *	Name encoded markers are a failsafe mechanism to ensure marker mappability.
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
			
			/**
			 *	Commits staged changes made to source, i.e. added markers and attributes, to filepath.
			 *	@param	outputPath	
			 */
			void commitToSingleFile(filesystem::path outputPath){
				// returns empty if path is on same dir.
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));	
				if(!filesystem::exists(outputPath)){filesystem::create_directories(outputPath.parent_path());}
				std::error_code EC;
				llvm::raw_fd_ostream outFile(outputPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {llvm::errs() << "commitToSingleFile: error opening output file: " << EC.message() << "\n"; return;}
				rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(outFile);
				outFile.close();	
			}
			
			/**
			 *	Commits file to its relative path counterpart in outputDir.
			 *
			 *	Example:
			 *	For some file test.c (recorded in the rewriter), that was found in inputDir.
			 *
			 *	inputDir:	 baseA/path/to/dir/
			 *	outputDir:	 baseB/
			 *
			 *	Result:
			 *	File test.c get written to baseB/path/to/dir/test.c.
			 *
			 *	@param	inputDir	directory of queuefile
			 *	@param	outputDir	directory of basedir relocation
			 */
			void commitToQueueFile(filesystem::path inputDir, filesystem::path outputDir){
				StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(SM.getMainFileID()));
				filesystem::path outPath = outputDir / filesystem::relative(FilePath.str(), inputDir);
				if(!filesystem::exists(outPath)){filesystem::create_directories(outPath.parent_path());}
				std::error_code EC;
				llvm::raw_fd_ostream outFile(outPath.string(),EC, llvm::sys::fs::OF_Text);
				if (EC) {llvm::errs() << "commitToQueueFile: error opening output file: " << EC.message()<< "\n"; return;}
				rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(outFile);
				outFile.close();	
			}
		
		bool VisitStmt(Stmt *S) {
			if (SM.isInMainFile(S->getBeginLoc())){				
				//-- Start marker safety checks
				if(!stmtHasValidChildCriteria(S)){return true;} // TODO: might be redundant

				unsigned stmtStart = SM.getFileOffset(SM.getSpellingLoc(S->getBeginLoc()));
				if (macroRecorder->isOffsetInsideMacro(stmtStart)) {
				    return true;   // cannot inject before, because start is inside a macro
				}

				// Clang Macro expansion id check.
				if(S->getBeginLoc().isMacroID() || S->getBeginLoc().isInvalid()){return true;}
				DynTypedNodeList Parents = context.getParents(*S); // TODO: migrate to parentStack method?
				for (const auto &Parent : Parents) {
					if (Parent.getSourceRange().getBegin().isMacroID()) {return true;}
				}

				// TODO: we should decouple format mode, thus the remove of this snippet is likely.
				if(!clangFormatMode){
					if(Parents.empty()) {
						return true;
					} else if(!Parents[0].get<CompoundStmt>()){
						return true;
					}
				}
				
				//-- End safety checks;
				
				switch (S->getStmtClass()) {
					case Stmt::CallExprClass: 		processCallExpr(cast<CallExpr>(S)); 					break;
					case Stmt::CaseStmtClass: 		processCaseStmt(cast<CaseStmt>(S)); 					break;
					case Stmt::DeclStmtClass: 		processDeclStmt(cast<DeclStmt>(S)); 					break;
//					case Stmt::DefaultStmtClass:	processDefaultStmt(cast<DefaultStmt>(S));				break;
//					case Stmt::DoStmtClass:			processDoStmt(cast<DoStmt>(S));						break;
					case Stmt::ForStmtClass: 		processForStmt(cast<ForStmt>(S)); 						break;
					case Stmt::GotoStmtClass: 		processGotoStmt(cast<GotoStmt>(S)); 					break;
					case Stmt::IfStmtClass: 		processIfStmt(cast<IfStmt>(S)); 						break;
					case Stmt::SwitchStmtClass: 	processSwitchStmt(cast<SwitchStmt>(S));				break;
					case Stmt::WhileStmtClass: 		processWhileStmt(cast<WhileStmt>(S)); 					break;
					case Stmt::BinaryOperatorClass:	processBinaryOperatorStmt(cast<BinaryOperator>(S)); 	break;
					default: break;
				}
			}
			return true;
		}
		
//------	Declarations	------
		bool VisitDecl(Decl *D){
			if (SM.isInMainFile(D->getLocation())){
				unsigned declStart = SM.getFileOffset(SM.getSpellingLoc(D->getBeginLoc()));
				
				if (macroRecorder->isOffsetInsideMacro(declStart)) {
				    return true;   // inside a macro
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
		
		/**
		 * TODO: 
		 * - Safe inline removals: https://stackoverflow.com/questions/79589301/will-removal-of-all-inline-keywords-from-my-code-leave-my-c-program-semantical
		 * - Make function attributes optional, because headers macro's might already be enough for optimization control.
		 *
		 * Useful info:
		 * https://clang.llvm.org/docs/AttributeReference.html#optnone
		 */
		void processFunctionDecl(FunctionDecl* FD){
			addOrCreateGNUOptnone(FD, rewriter, SM, CI.getLangOpts());			
		}
		
//------	Statements		------
		void processDeclStmt(DeclStmt* DS) {
			DynTypedNodeList Parents = context.getParents(*DS);
			for (const auto &Parent : Parents) {
				if (const auto *ParentStmt = Parent.get<Stmt>()) {
					// If parent node is one of the following, then we skip the marking.
					// Otherwise we wrongly mark the parameters within these constructs, leading to invalid syntax.
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
		 * 	Consideration: Mark cases as well or simply retrieve case info from within the switch stmt.
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
		 * Compilers can discard these upon assembly generation.
		 */
		void processCaseStmt(CaseStmt* CS){
			rewriter.InsertText(
			CS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		void processDefaultStmt(DefaultStmt* DS){
			rewriter.InsertText(
			DS->getBeginLoc(),
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
		
		void processBinaryOperatorStmt(BinaryOperator* BO){
			rewriter.InsertText(BO->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
		
		void processDoStmt(DoStmt* DS){
			rewriter.InsertText(DS->getBeginLoc(),
			synthesizeCallMarkerString(), 
			false, 
			true
			);
		}
};
