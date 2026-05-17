#pragma once

#include <nlohmann/json.hpp>
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "llvm/Support/raw_ostream.h"

using namespace nlohmann;
using namespace clang;

class OracleIndexVisitor: public RecursiveASTVisitor<OracleIndexVisitor> {
	private:
		json& markerFormat;
		SourceManager& SM;
	public:
		OracleIndexVisitor(SourceManager& SM, json& markerFormat) : 
			markerFormat(markerFormat), SM(SM)
			{}
			
			bool VisitStmt(Stmt *S) {
				// if marker is visited in oracle or decom then establish mapping
				llvm::outs() << S->getStmtClassName() << " \n";
				return true;
			}
};