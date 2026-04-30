#pragma once
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Frontend/FrontendAction.h"
#include "clang/Tooling/Tooling.h"

#include "clang/Frontend/FrontendActions.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"

#include "MacroFNs.hpp"

using namespace clang;

void processDeclStmt(DeclStmt* DS, ASTContext* context, const clang::syntax::TokenBuffer &Tokens, SourceManager &SM){
	llvm::outs() << "DeclStmt found at " << DS->getSourceRange().getBegin().printToString(SM) << "\n";
//							SourceLocation Begin = DS->getBeginLoc();
//							SourceLocation ExpansionLoc = SM.getExpansionLoc(Begin);
//							    // Use the expansion location to check if it's in the main file
//							    if (SM.isWrittenInMainFile(ExpansionLoc)) {
//							        auto info = detectMacrosAroundStatement(DS, Tokens, SM, context->getLangOpts());
//							        printMacroExpansionInfo(info, Tokens, SM);
//							    }
	printMacroExpansionInfo(
		//detectMacrosAroundStatement(cast<SwitchStmt>(S), Tokens,SM, context->getLangOpts()),
		detectAdjacentMacros(DS, Tokens,SM, context->getLangOpts()),
		Tokens,
		SM
	);
	
	if (auto macroName = getMacroNameIfStmtStartsWithMacro(DS, Tokens, SM, context->getLangOpts())) {
	        llvm::outs() << "DeclStmt originates from macro: " << *macroName << "\n";
	}
}