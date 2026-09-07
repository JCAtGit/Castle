#pragma once

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Preprocessor.h"


using namespace clang;

/**
 * 	TODO:
 *		1. Afhankelijk vd marker flag, remove or insert can hier een verschillende instantie worden meegegeven.
 *		De naam voor de codemarker inserter dus veralgemeniseren.
 */
 class CodeRemoverVisitor: public RecursiveASTVisitor<CodeRemoverVisitor> {
 	private:
 		CodeMarkerRemover& remover;
 	public:
 		explicit CodeRemoverVisitor(CodeMarkerRemover &remover) :remover(remover){}
 
		bool VisitDecl(Decl *D) {
			remover.processDecl(D);
			return true;
		}
	
		bool VisitStmt(Stmt *S) {
			remover.processStmt(S);
			return true;
		}
		
		// NOTICE: Should be performed after rewriter is done.
		void removeMarkerHeaderInclusion(){
			llvm::outs() << "Removing marker header inclusion directive. \n";
			removeFirstLineIfContains(rewriter, SM, cfg.markerDefinitionFileName);
		}
		
		void commitToFile(){
			llvm::outs() << "Start commit phase" << "\n";
			// returns empty if path is on same dir.
			FileID MainFileID = SM.getMainFileID();
			StringRef FilePath = SM.getFilename(SM.getLocForStartOfFile(MainFileID));

			fs::path outPath = cfg.outputDir / rel_outputPath(cfg.inputDir, FilePath.str());
			if (outPath.empty() || outPath == "." || outPath == "./") {
			    outPath = FilePath.str(); // Handle appropriately – maybe use just the filename
			} 
			llvm::outs() << "Commiting changes from: " << FilePath << " -> " << outPath << "\n";
						
			// committing buffered changes to file
			if(!std::filesystem::exists(outPath.string())){
				fs::create_directories(outPath.parent_path());
			}
			// write to file and mutate with srcExt if desired.
			std::error_code EC;
			llvm::raw_fd_ostream OutFile(
				fs::path(outPath).replace_filename(
					outPath.stem().string() + 
//					cfg.srcExt + 
					outPath.extension().string()
				).string(),
				EC, 
				llvm::sys::fs::OF_Text
			);

			if (EC) {
				llvm::errs() << "Error opening output file: " << EC.message()<< "\n";
				return;
			}

			// Write buffered changes to file.
			rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(OutFile);
			OutFile.close();	
		}
		
		template<typename NodeType> // only decl and stmt
		void deleteNode(NodeType* ASTNode) {
			SourceRange range = ASTNode->getSourceRange();
	//		SourceLocation end;
			SourceLocation semicolon = Lexer::findLocationAfterToken(range.getEnd(), tok::semi, context->getSourceManager(), context->getLangOpts(), false);
			if (semicolon.isValid()) {
				SourceLocation afterSemi = Lexer::getLocForEndOfToken(semicolon, 0, context->getSourceManager(), context->getLangOpts());
				clang::Rewriter::RewriteOptions opts;
				opts.RemoveLineIfEmpty = true; // optie heeft een FIXME in de docs, bug reports tonen dat deze plaatsvindt bij meerdere edits rond dezelfde regel.
				R->RemoveText(SourceRange(range.getBegin(), afterSemi),opts);
			} 
		}
		
		static void removeDependencyFiles(struct config& cfg){
			llvm::outs() << "Removing definition header and source file. \n";
			fs::remove((cfg.inputDir / (cfg.markerDefinitionFileName + ".h")).string());  
			fs::remove((cfg.inputDir / (cfg.markerDefinitionFileName + ".c")).string());  
		}
		
		virtual void processFunctionDecl(FunctionDecl* FD){
			if (FD->getNameAsString() != markerName) return;
			if (FD->getNumParams() < 1) return;
			deleteNode(FD);
		}
		
		virtual void processCallExpr(CallExpr* CE){
		  	const clang::FunctionDecl *callee = CE->getDirectCallee();
		   	if (!callee) return; // indirect call (function pointer, etc.)
			
		   	std::string funcName = callee->getNameAsString();
		   	if (funcName != markerName) return;
		   	if (CE->getNumArgs() < 1) return;		
			deleteNode(CE);
		}
};
