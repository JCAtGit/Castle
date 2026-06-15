#pragma once

#include <boost/math/distributions/binomial.hpp>
#include "nlohmann/json.hpp"

using namespace nlohmann;

/**
 * 	general construct recovery stats
 *		- oracle markers present
 *		- decom markers found
 *			- also include duplicates
 *	per stmt recovery stats
 *		- general attribute recovery stats
 *		- associated recovered decom constructs and their numbers
 *			- most common recovered decom construct given some oracle construct
 * 
 */
void statisticizeFindings(json& jsonData, json& summary){
	
}

void statisticizeEvals(json& jsonData, json& summary){
	
}

void statisticizeDevReports(json& jsonData, json& summary){
	
}