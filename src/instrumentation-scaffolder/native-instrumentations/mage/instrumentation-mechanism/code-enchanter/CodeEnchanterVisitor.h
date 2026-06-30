#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

#include <filesystem>
#include <vector>
#include <string>
#include <helper.hpp>
#include "../preprocessor/MacroExpansionRecorder.hpp"
#include "predicates-stmt.hpp"
#include "predicates-decl.hpp"

using namespace clang;
using namespace std;
using namespace nlohmann;

/**
 *	Visitor class for source code. 
 *	Implements: Marker, Attribute Graft Enhancements (MAGE)
 */
class CodeEnchanterVisitor: public RecursiveASTVisitor<CodeEnchanterVisitor> {
	private:
		Rewriter& rewriter;
		MacroExpansionRecorder* macroRecorder = nullptr;
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
			MacroExpansionRecorder* macroRecorder,
			json &markerFormatCfg, 
			int &idCounter,
			bool &nameEncodedMarkers,
			bool &clangFormatMode
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
				if(!filesystem::exists(outputPath)){
					filesystem::create_directories(outputPath.parent_path());
				}
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
		
			/**
			 *	Checks if a FunctionDecl has conflicting attributes.
			 *	If attributes conflicting with directives to promote no optimization are found, the FunctionDecl is left untouched.
			 * 	In other cases, the attributes will be added.
			 */
			void addOrCreateGNUOptnone(
				FunctionDecl *FD, 
				Rewriter &Rewriter,
				const SourceManager &SM, 
				const LangOptions &LangOpts
			) {
			    // Conflict detection using Clang attribute methods, check attributes that are incompatible with optnone or if attribute already exists.
			    if (FD->hasAttr<OptimizeNoneAttr>() || FD->hasAttr<AlwaysInlineAttr>() || FD->hasAttr<NakedAttr>() ||FD->hasAttr<WeakAttr>() || FD->hasAttr<WeakRefAttr>()) {
			        return; 
				}
					
				// Only rewrite if this is a function definition (has a body) AND it is at file scope.
				if (!FD->isThisDeclarationADefinition()) return;
				if (!FD->getLexicalDeclContext()->isFileContext()) return;
			
				// Additionally, ensure the source location is in a file with a recognized extension (e.g., .c or .h) but not in a system header.
				if (SM.isInSystemHeader(FD->getLocation())) return;
				if (SM.isInExternCSystemHeader(FD->getLocation())) return;
			
				// Reject if the declaration is inside a struct/union (shouldn't happen for FunctionDecl, but double-check)
				if (FD->getDeclContext()->isRecord()) return;
				
			    // Helper to check if a source location is safe for rewriting (not inside a macro expansion and maps to a file location).
			    auto isLocationSafe = [&](SourceLocation Loc) -> bool {
			        if (Loc.isInvalid()) return false;
			        if (Loc.isMacroID()) return false;
			        CharSourceRange TokenRange = CharSourceRange::getTokenRange(Loc, Loc);
			        CharSourceRange FileRange = Lexer::makeFileCharRange(TokenRange, SM, LangOpts);
			        if (FileRange.isInvalid()) return false;
			        return FileRange.getBegin().isFileID();
			    };
			
			    auto isRangeSafe = [&](SourceRange R) -> bool {
			        return R.isValid() && isLocationSafe(R.getBegin()) && isLocationSafe(R.getEnd());
			    };
			
			    // Try to merge into an existing GNU __attribute__((...)).
				// For functions without a body declarations we insert a new attribute.
			    if (FD->isThisDeclarationADefinition()) {
			        // Find the opening brace location of the function body.
			        Stmt *Body = FD->getBody();
			        if (Body) {
			            SourceLocation BraceLoc = Body->getBeginLoc();
			            // Find an existing GNU attribute that ends before the brace.
			            Attr *TargetAttr = nullptr;
			            for (auto *A : FD->attrs()) {
			                if (A->getSyntax() != AttributeCommonInfo::AS_GNU)
			                    continue;
			                SourceRange R = A->getRange();
			                if (!isRangeSafe(R))
			                    continue;
			                if (SM.isBeforeInTranslationUnit(R.getEnd(), BraceLoc)) {
			                    TargetAttr = A;
			                    break; // take the first such attribute
			                }
			            }
			
			            if (TargetAttr) {
			                SourceRange range = TargetAttr->getRange();
			                CharSourceRange charRange = Lexer::getAsCharRange(range, SM, LangOpts);
			                StringRef attrText = Lexer::getSourceText(charRange, SM, LangOpts);
			                if (!attrText.empty() && !attrText.contains("optnone")) {
			                    std::string newAttrText;
			                    if (attrText.find("()") != StringRef::npos) {
			                        // Empty argument list: __attribute__(())
			                        newAttrText = "__attribute__((optimize(\"O0\"), optnone))";
			                    } else {
			                        // Insert "optnone, " after the second '('.
			                        size_t start = attrText.find('(');
			                        if (start != StringRef::npos) {
			                            start = attrText.find('(', start + 1);
			                            if (start != StringRef::npos) {
			                                newAttrText = attrText.str();
			                                newAttrText.insert(start + 1, "optimize(\"O0\"), optnone, ");
			                            }
			                        }
			                    }
			                    if (!newAttrText.empty() && isRangeSafe(range))
			                        Rewriter.ReplaceText(range, newAttrText);
			                }
			                return;
			            }
			        }
			    }
			
				// Insert a new attribute after the return type.
				SourceRange retRange = FD->getReturnTypeSourceRange();
				if (retRange.isInvalid()) return;
			
				SourceLocation insertLoc = Lexer::getLocForEndOfToken(retRange.getEnd(), 0, SM, LangOpts);
				if (insertLoc.isInvalid() || !isLocationSafe(insertLoc)) return;
			
				Rewriter.InsertText(insertLoc, " __attribute__((optimize(\"O0\"), optnone))");
			}
		
		bool VisitStmt(Stmt *S) {
			if (SM.isInMainFile(S->getBeginLoc())){				
				//-- Start marker safety checks
				if(!stmtHasValidChildCriteria(S)){return true;}
				if (macroRecorder->hasOverlap(S->getSourceRange())) {
					return true;
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
