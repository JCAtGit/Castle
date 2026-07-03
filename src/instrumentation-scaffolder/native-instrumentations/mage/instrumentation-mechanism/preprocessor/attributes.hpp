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
    if (
		// Conflict detection with attributes that are incompatible with optnone.
		FD->hasAttr<MinSizeAttr>() 		||
		FD->hasAttr<OptimizeNoneAttr>() || 
		FD->hasAttr<AlwaysInlineAttr>() || 
		FD->hasAttr<NakedAttr>() 		||
		FD->hasAttr<WeakAttr>() 		|| 
		FD->hasAttr<WeakRefAttr>() 		|| 
		FD->hasAttr<GNUInlineAttr>() 	|| 
		FD->isInlineSpecified() 		|| 
		FD->isInlined() 				||
		
		// Only rewrite if this is a function definition (has a body) AND it is at file scope.
		!FD->isThisDeclarationADefinition() 				|| 
		!FD->getLexicalDeclContext()->isFileContext() 		||
		
		// Additionally, ensure the source location is in a file with a recognized extension (e.g., .c or .h) but not in a system header.
		SM.isInSystemHeader(FD->getLocation()) 		|| 
		
		// Reject if the declaration is inside a struct/union (shouldn't happen for FunctionDecl, but double-check)
		SM.isInExternCSystemHeader(FD->getLocation())	||
		FD->getDeclContext()->isRecord()
	) {return;}
	
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

	