#include "clang/AST/Expr.h"
#include "clang/AST/Stmt.h"
#include <iostream>
#include <nlohmann/json.hpp>
#include <string>
#include <unordered_map>

using namespace nlohmann;

/**
 *	NOTICE: Work in progress to extract procedures for more streamlined use through libraries.
 *	Currently unused.
 *	
 *	TODO: 
 *		Develop function to detect markers without debug information.
 */

 using namespace std;
 using namespace clang;
 
/**
 *	TODO: 
 *		add detection for when debug information is disabled or absent.
 *		decouple nameEncodedMarkers, magestring from function definition.
 *
 *	Heuristic function to determine the identity of a marker function from a CallExpr.
 *	When nameEncodedMarkers are enable, indexes will extracted from the name instead.
 *	@param potentialMarker 		Investigated CallExpr for marker characteristics.
 *	@param markerFormatConfig	
 */
int getMageIndex(CallExpr* potentialMarker, json &markerFormatConfig, bool nameEncodedMarkers = true){
	auto DC = potentialMarker->getDirectCallee();
	if (!DC) return -1;
	string namestring = DC->getNameInfo().getName().getAsString();
	string magestring = markerFormatConfig["name"].get<string>();
	
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



