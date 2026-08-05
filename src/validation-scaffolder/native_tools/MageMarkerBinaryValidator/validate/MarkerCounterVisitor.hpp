#pragma once

#include "nlohmann/json.hpp"
#include "clang/AST/ASTContext.h"
#include "clang/AST/Expr.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/AST/Stmt.h"
#include "clang/Basic/SourceManager.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Rewrite/Core/Rewriter.h"
#include "clang/Lex/Lexer.h"

#include <filesystem>
#include <string>
#include "paths.hpp"
using namespace clang;
using namespace std;
using namespace nlohmann;

/**
 *	Visitor class for source code. 
 *	Implements: Marker, Attribute Graft Enhancements (MAGE)
 */
class MarkerCounterVisitor: public RecursiveASTVisitor<MarkerCounterVisitor> {
	private:
		json &markerFormat, &resultFile;
		bool &nameEncodedMarkers;
		ASTContext *selectedAST;
		SourceManager *selectedSM;
		string magestring;
	public:
		MarkerCounterVisitor(
			json &markerFormat, 
			json &resultFile,
			bool &nameEncodedMarkers
		): markerFormat(markerFormat), resultFile(resultFile), nameEncodedMarkers(nameEncodedMarkers) {
			magestring = markerFormat["name"].get<string>();;
		}
	
		void selectAST(ASTContext *otherAST){
			selectedAST = otherAST;
			selectedSM = &(otherAST->getSourceManager());
		}
		
			
		/**
		 *	Heuristic function to determine the identity of a marker function from a CallExpr.
		 *	When nameEncodedMarkers are enable, indexes will extracted from the name instead.
		 */
		int getMageIndex(CallExpr* potentialMarker){
			auto DC = potentialMarker->getDirectCallee();
			if (!DC) return -1;
			string namestring = DC->getNameInfo().getName().getAsString();

			if (namestring.compare(0, magestring.length(), magestring) != 0){return -1;} 		
			if (potentialMarker->getNumArgs() > 0 && !nameEncodedMarkers){
				auto IL = dyn_cast<IntegerLiteral>(potentialMarker->getArg(0)->IgnoreImpCasts());
				if (IL) {
					llvm::APInt Value = IL->getValue();
					return Value.getSExtValue();
				}
			} else if(nameEncodedMarkers){
				string postfix = namestring.substr(magestring.length());
				if (postfix.empty() || !std::all_of(postfix.begin(), postfix.end(), ::isdigit)) {return -1;}
				return std::stoi(postfix);
			}
			return -1;
		}
			
		bool VisitCallExpr(CallExpr *CE){
			if (selectedSM->isInMainFile(CE->getBeginLoc())){
				int foundIndex = getMageIndex(CE);	
				if(foundIndex != -1){
					resultFile[foundIndex] = false; // we just insert the entry for existence, only after verification is it true
				}
			}
			return true;
		}
};
