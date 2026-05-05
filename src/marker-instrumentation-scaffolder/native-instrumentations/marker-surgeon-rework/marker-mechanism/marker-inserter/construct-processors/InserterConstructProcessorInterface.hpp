#pragma once

#include "clang/AST/Decl.h"
#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Tooling/Syntax/Tokens.h"
#include <optional>

using namespace clang;
using namespace std;

class InserterConstructProcessor{
	private:
		ASTContext* Context;
		Rewriter* R;
		int MarkerID;
		string markerName;
		string markerType;
		string markerSignature;
		string markerDefinition;
		string markerEndString;
		// config unused
		int markerDepth = 0; // unused for now
		
	public:
		virtual ~InserterConstructProcessor() = default;
		virtual void setTokenBuffer(syntax::TokenBuffer tokenBuffer) = 0;
		virtual syntax::TokenBuffer& getTokenBuffer() = 0;
		
		// Marker formatting
		virtual string synthesizeMarkerDeclarationString() = 0;
	    virtual string synthesizeCallMarkerString() = 0;
		virtual string synthesizeMarkerDefinitionString() = 0;
		virtual string synthesizeMarkerDependencyString(string fileIn, struct config *cfg) = 0;
		
		// Declarations.
		virtual void processDecl(Decl* D) = 0;
		virtual void processFunctionDecl(FunctionDecl* FD) = 0;
		
		// Statements.
		virtual void processDeclStmt(DeclStmt* DS) = 0;
		virtual void processForStmt(ForStmt* FS) = 0;
		virtual void processWhileStmt(WhileStmt* WS) = 0;
		virtual void processIfStmt(IfStmt* IS) = 0;
		virtual void processSwitchStmt(SwitchStmt* SS) = 0;
		virtual void processCaseStmt(CaseStmt* CS) = 0;
		virtual void processCallExpr(CallExpr* CE) = 0;
		virtual void processGotoStmt(GotoStmt* GS) = 0;
};
