#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/Attr.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

using namespace clang;
using namespace std;

/**
 *	Attribute documentation:
 *	gcc: 	https://gcc.gnu.org/onlinedocs/gcc-15.1.0/gcc/Common-Function-Attributes.html
 *	clang:	https://clang.llvm.org/docs/AttributeReference.html#optnone
 *	msvc:	https://learn.microsoft.com/en-us/cpp/c-language/function-attributes?view=msvc-170
 * 	
 *	Some attributes were found through searching the IDE API.
 */

// Conflict detection with attributes that are incompatible with optnone and similar or already has something like optnone.
bool hasAttrConflictsWithOptNone(FunctionDecl *FD){
	return 	FD->hasAttr<MinSizeAttr>() 		||
			FD->hasAttr<AlwaysInlineAttr>() || 
			FD->hasAttr<NakedAttr>() 		||
			FD->hasAttr<WeakAttr>() 		|| 
			FD->hasAttr<WeakRefAttr>() 		|| 
			FD->hasAttr<GNUInlineAttr>() 	|| 
			FD->isInlineSpecified() 		|| 
			FD->isInlined() 				;
}

/**
 * 	TODO: Decouple hardcoded attributes enhancements and make flexible for multiple optimization options.
 *
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
	// non macro checks
    if (hasAttrConflictsWithOptNone(FD)				||
		FD->hasAttr<OptimizeNoneAttr>() ||

		// Only rewrite if this is not a function declaration only, so we don't cause unexpected effects with later definitions.
		!FD->isThisDeclarationADefinition() 				|| 
		
		// TODO: misschien is de mainfile check al genoeg, dan kunnen de Filecontext en SM checks weg.
		// Should be in file scope
		!FD->getLexicalDeclContext()->isFileContext() 		||
		// Ensure the source location is not in a system header.
		SM.isInSystemHeader(FD->getLocation()) 		|| 
		SM.isInExternCSystemHeader(FD->getLocation())	||
		
		// Reject if the declaration is inside a struct/union (shouldn't happen for FunctionDecl, but double-check)
		FD->getDeclContext()->isRecord()
	) {return;}
	
    // Helper to check if a source location is safe for rewriting (not inside a macro expansion and maps to a file location).
	// TODO: evaluate if macro checks are still necessary when we do this at the decl traversal level?
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
    // Find the opening brace location of the function body.
    Stmt *Body = FD->getBody();
    if (Body) {
        SourceLocation BraceLoc = Body->getBeginLoc();
        // Find an existing GNU attribute that ends before the brace.
        Attr *TargetAttr = nullptr;
        for (auto *A : FD->attrs()) {
            if (A->getSyntax() != AttributeCommonInfo::AS_GNU) continue;
            SourceRange R = A->getRange();
            if (!isRangeSafe(R)) continue;
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
    
	SourceRange retRange = FD->getReturnTypeSourceRange();	// Insert a new attribute after the return type.
	if (retRange.isInvalid()) return;
	SourceLocation insertLoc = Lexer::getLocForEndOfToken(retRange.getEnd(), 0, SM, LangOpts);
	if (insertLoc.isInvalid() || !isLocationSafe(insertLoc)) return;

	Rewriter.InsertText(insertLoc, " __attribute__((optimize(\"O0\"), optnone))");
}

	