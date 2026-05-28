
#include "clang/Lex/PPCallbacks.h"
#include "clang/Basic/SourceLocation.h"   // for SourceLocation
#include "clang/Lex/MacroArgs.h"          // for MacroArgs
#include "clang/Lex/MacroInfo.h"          // for MacroInfo
#include "clang/Lex/Preprocessor.h"       // for Preprocessor (if needed)
using namespace clang;

class CustomPPCallbacks : public clang::PPCallbacks {
	public:
	// Called when a macro is defined
	void MacroDefined(const clang::Token &MacroNameTok, const clang::MacroDirective *MD) override {
    	llvm::outs() 	<< "Macro defined: " 
        				<< MacroNameTok.getIdentifierInfo()->getName() 
						<< "\n";
	}

   // Called when a macro is expanded (Clang 21 signature)
	void MacroExpands(
		const clang::Token &MacroNameTok,
		const clang::MacroDefinition &MD,
		clang::SourceRange Range,
		const clang::MacroArgs *Args) override {
       	// Note: in Clang 21, the fourth parameter is a pointer to MacroArgs,
       	// and the third is a SourceRange (not TokenRange).
       	llvm::outs() 	<< "Macro expanded: " 
                    	<< MacroNameTok.getIdentifierInfo()->getName() 
						<< "\n";
       	// Use `Range` to get source locations of the expansion.
   	}
private:

};