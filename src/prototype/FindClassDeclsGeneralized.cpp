#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "marker-injector/filereader/filereader.h"

using namespace clang;

class CustomASTVisitor : public RecursiveASTVisitor<CustomASTVisitor> {
	public:
    	explicit CustomASTVisitor(ASTContext *Context) : Context(Context) {}
		
		// General Visit Methods; Decl, Stmt, Type and Attr.
		bool VisitDecl(Decl *D) {
		        SourceManager &SM = Context->getSourceManager();
		        SourceLocation Loc = D->getLocation();
		        
		        if (!Loc.isValid()) return true;
		        
		        // Only process declarations from the main file
		        if (!SM.isInMainFile(Loc)) {
		            return true;
		        }
		        
		        llvm::outs() << "Decl: " << D->getDeclKindName();
		        if (NamedDecl *ND = dyn_cast<NamedDecl>(D)) {
		            llvm::outs() << " | Name: " << ND->getNameAsString();
		        }
		        llvm::outs() << "\n";
		        
		        return true;
		    }
		    
		    bool VisitStmt(Stmt *S) {
		        SourceManager &SM = Context->getSourceManager();
		        SourceLocation Loc = S->getBeginLoc();
		        
		        if (!Loc.isValid()) return true;
		        
		        // Only process statements from the main file
		        if (!SM.isInMainFile(Loc)) {
		            return true;
		        }
		        
		        llvm::outs() << "Stmt: " << S->getStmtClassName();
		        
		        // If it's a call expression, show what's being called
		        if (CallExpr *Call = dyn_cast<CallExpr>(S)) {
		            if (FunctionDecl *FD = Call->getDirectCallee()) {
		                llvm::outs() << " | Calls: " << FD->getNameAsString();
		            }
		        }
		        
		        llvm::outs() << "\n";
		        
		        return true;
		    }

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
// 			// ... and many more

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
//			// ... and many more

	private:
    	ASTContext *Context;
};



class CustomASTConsumer : public clang::ASTConsumer {
	public:
  		explicit CustomASTConsumer(ASTContext *Context): Visitor(Context) {}
  		virtual void HandleTranslationUnit(clang::ASTContext &Context) {
		    // Print the color-coded ast-tree to terminal
//		    Context.getTranslationUnitDecl()->dump();
		    
		    // Then run your custom visitor
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
			// Tell the preprocessor to ignore includes
			Compiler.getPreprocessor().SetSuppressIncludeNotFoundError(true);
			
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

	llvm::outs() << "Program start. \n";
	std::string fileStrData;
	std::string InputFile = argv[1];
//	std::string OutputFile = argv[2];
	
  	if (argc > 1) {
  		fileStrData = file2string(InputFile);
		
		llvm::outs() << "Argument found, count: " + std::to_string(argc) + "\n\n";
		llvm::outs() << fileStrData;
		llvm::outs() << "\n";
    	clang::tooling::runToolOnCodeWithArgs(
    		std::make_unique<CustomASTFrontAction>(), 
    		fileStrData,
    		Args,
			InputFile.c_str()
    	);
  	}
	llvm::outs() << "Program end. \n";
};
