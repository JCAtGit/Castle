#pragma once

#include "clang/AST/ASTContext.h"
#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/ParentMapContext.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"

using namespace std;
using namespace clang;

// Check if declaration (or its location) comes from a macro
bool declFromMacro(const Decl *D, const ASTContext &Ctx) {
    SourceLocation Loc = D->getLocation();
    if (Loc.isMacroID()) return true;
    // Also check the expansion range – some macro tokens survive as macroIDs
    const SourceManager &SM = Ctx.getSourceManager();
    return SM.isMacroBodyExpansion(Loc) || SM.isMacroArgExpansion(Loc);
}



