
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"

#include "../../../include/helper.hpp"
#include "CustomPPCallbacks.hpp"
#include "MacroFNs.hpp"
#include "VisitorProcessor.hpp"

using namespace clang;

class CustomASTVisitor : public RecursiveASTVisitor<CustomASTVisitor> {
	public:
		explicit CustomASTVisitor(ASTContext *Context, syntax::TokenBuffer &Tokens): context(Context), SM(Context->getSourceManager()), Tokens(Tokens) {}

			// https://clang.llvm.org/doxygen/RecursiveASTVisitor_8h_source.html		    
		    bool VisitStmt(Stmt *S) {
				if (SM.isInMainFile(S->getBeginLoc())){
				//... process statements here
				switch (S->getStmtClass()) {
					case Stmt::IfStmtClass:
					    break;
					case Stmt::WhileStmtClass:
						break;
					case Stmt::DeclStmtClass:
						processDeclStmt(cast<DeclStmt>(S), context, Tokens, SM);
					    break;
					case Stmt::ForStmtClass:
						break;
					case Stmt::CallExprClass:
					    break;
					case Stmt::SwitchStmtClass:
						break;
					case Stmt::CaseStmtClass:
						break;
					default:
					  	break;
				}
			}
      		return true;
		}
	private:
    	ASTContext *context;
		syntax::TokenBuffer &Tokens;
		SourceManager &SM;
};



class CustomASTConsumer : public clang::ASTConsumer {
	public:
	explicit CustomASTConsumer(ASTContext *context, std::unique_ptr<syntax::TokenCollector> collector): context(context), collector(std::move(collector)) {}	
  		
	virtual void HandleTranslationUnit(clang::ASTContext &Context) {
		    // Print the color-coded ast-tree to terminal
//		    Context.getTranslationUnitDecl()->dump();
		    
			// syntax::TokenBuffer stores the tokenized results of a source file
//			It captures two distinct token streams:
//			Expanded Tokens: The final stream after all macro replacements and preprocessing directives have been applied, representing what the compiler's parser sees.
//			Spelled Tokens: The exact token sequence as they appear in the original source code files, before any macro expansion occurs.
		if (!collector) return;
		
       // 1. Consume the collector to get the TokenBuffer
       tokenBuffer = std::move(*collector).consume();

       // 2. Index the tokens for faster range lookups
       tokenBuffer->indexExpandedTokens();

       // 3. Now construct the visitor with both context and token buffer
       CustomASTVisitor visitor(context, *tokenBuffer);
       visitor.TraverseDecl(Context.getTranslationUnitDecl());
	}
	private:
		ASTContext *context;
		std::unique_ptr<syntax::TokenCollector> collector;
		std::optional<syntax::TokenBuffer> tokenBuffer;  // Stored as member to keep it alive

};

class CustomASTFrontAction : public clang::ASTFrontendAction {
	public:
  		virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    		clang::CompilerInstance &Compiler, 
			llvm::StringRef InFile
		) {
			Compiler.getPreprocessor().SetSuppressIncludeNotFoundError(true);
//			Compiler.getPreprocessor().addPPCallbacks(std::make_unique<CustomPPCallbacks>());
			
			auto Collector = std::make_unique<syntax::TokenCollector>(Compiler.getPreprocessor());
			// Implements factory pattern and returns pointer to created object.
    		return std::make_unique<CustomASTConsumer>(&Compiler.getASTContext(), std::move(Collector));
  		}
};

int main(int argc, char **argv) {
	std::vector<std::string> Args = {
	    "-xc",              	// Treat the input as C code
	    "-std=c11",         	// Use a specific C standard
		"-nostdinc",     		// Don't search for system headers
        "-Wno-all",      		// Suppress all warnings
	};

	llvm::outs() << "/ClangPPTokenAnalyser start. \n";
	std::string fileStrData;
	std::string InputFile = argv[1];
//	std::string OutputFile = argv[2];
	
  	if (argc > 1) {
  		fileStrData = file2string(InputFile);
		
		llvm::outs() << "Argument found, count: " + std::to_string(argc) + "\n\n";
//		llvm::outs() << fileStrData;
//		llvm::outs() << "\n";
    	clang::tooling::runToolOnCodeWithArgs(
    		std::make_unique<CustomASTFrontAction>(), 
    		fileStrData,
    		Args,
			InputFile.c_str()
    	);
  	}
	llvm::outs() << "Program end. \n";
};
		