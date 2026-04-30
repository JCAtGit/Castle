#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"

#include "../marker-injector/filereader/filereader.h"
#include "CustomPPCallbacks.hpp"
using namespace clang;

class CustomASTVisitor : public RecursiveASTVisitor<CustomASTVisitor> {
	public:
    	explicit CustomASTVisitor(ASTContext *Context) : Context(Context) {}
		
		
//		 General Visit Methods; Decl, Stmt, Type and Attr.
//			bool VisitDecl(Decl *D) {
//				llvm::outs() << "Decl found: " << D->getID() << "\n";
//				return true;
//			}
//		    
//			bool visitDeclStmt(DeclStmt *DS){
//				llvm::outs() << "DeclStmt found: " << DS->getStmtClassName()<< "\n";
//				return true;
//			}
//			
//		    bool VisitStmt(Stmt *S) {
//				llvm::outs() << "Stmt found: " << S->getStmtClassName()<< "\n";
//				return true;
//			}

//			bool VisitType(Type *T){return true;}
//			bool VisitAttr(Attr *A){return true;}  
			
			// Visit Specific Declaration Types
//			bool VisitFunctionDecl(FunctionDecl *FD)
//			bool VisitVarDecl(VarDecl *VD)
//			bool VisitRecordDecl(RecordDecl *RD)      // struct/class/union
//			bool VisitEnumDecl(EnumDecl *ED)
//			bool VisitTypedefDecl(TypedefDecl *TD)
//			bool VisitNamespaceDecl(NamespaceDecl *ND)
//			bool VisitCXXRecordDecl(CXXRecordDecl *RD) // C++ classes
//			bool VisitClassTemplateDecl(ClassTemplateDecl *CTD)
// 			// ... and more

			// Visit Specific Statement/Expression Types
//			bool VisitCompoundStmt(CompoundStmt *CS)
//			bool VisitReturnStmt(ReturnStmt *RS)
//			bool VisitIfStmt(IfStmt *IS)
//			bool VisitForStmt(ForStmt *FS)
//			bool VisitCallExpr(CallExpr *CE)           // Function calls
//			bool VisitDeclRefExpr(DeclRefExpr *DRE)    // Variable references
//			bool VisitIntegerLiteral(IntegerLiteral *IL)
//			bool VisitStringLiteral(StringLiteral *SL)
//			bool VisitBinaryOperator(BinaryOperator *BO)
//			bool VisitUnaryOperator(UnaryOperator *UO)
//			// ... and more

	private:
    	ASTContext *Context;
};



class CustomASTConsumer : public clang::ASTConsumer {
	public:
  		explicit CustomASTConsumer(ASTContext *Context): Visitor(Context) {}
  		virtual void HandleTranslationUnit(clang::ASTContext &Context) {
		    // Print the color-coded ast-tree to terminal
//		    Context.getTranslationUnitDecl()->dump();
		    
		    // Then run custom visitor
		    Visitor.TraverseDecl(Context.getTranslationUnitDecl());
		}
	private:
  		CustomASTVisitor Visitor;
};

class CustomASTFrontAction : public clang::ASTFrontendAction {
	public:
  		virtual std::unique_ptr<clang::ASTConsumer> CreateASTConsumer(
    		clang::CompilerInstance &Compiler, 
			llvm::StringRef InFile
		) {
			// Preprocessor settings
			Compiler.getPreprocessor().SetSuppressIncludeNotFoundError(true);
			Compiler.getPreprocessor().addPPCallbacks(
						std::make_unique<CustomPPCallbacks>()
						);
			
			// syntax::TokenBuffer stores the tokenized results of a source file
//			It captures two distinct token streams:
//			Expanded Tokens: The final stream after all macro replacements and preprocessing directives have been applied, representing what the compiler's parser sees.
//			Spelled Tokens: The exact token sequence as they appear in the original source code files, before any macro expansion occurs.

			auto Collector = std::make_unique<syntax::TokenCollector>(Compiler.getPreprocessor());

			// Implements factory pattern and returns pointer to created object.
    		return std::make_unique<CustomASTConsumer>(&Compiler.getASTContext());
  		}
};


int main(int argc, char **argv) {
	
	// Arguments for clang ast-tool
	std::vector<std::string> Args = {
	    "-xc",                // Treat the input as C code
	    "-std=c11",           // Use a specific C standard, de grammar die we hebben is ook C11
		"-nostdinc",     // Don't search for system headers
        "-Wno-all",      // Suppress all warnings
        "-Wno-system-headers"  // Suppress system header warnings
	};

	llvm::outs() << "TraversalChecker start. \n";
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
