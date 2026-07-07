#pragma once
#include "matrix.hpp"
#include <cmath>
#include <string>
#include <sstream>

using namespace std;

/*
 *	We use viable comparisons for n, because that is the most direct metric.
 * 	Marker amount within any context is subject to many factors (repo, inlining, partial optimization, code usage/inclusion, separate test file marking, etc.), 
 *	which makes using this metric unsuitable.
 *
 */

/**
 *	success/failure condition expectation test for 3 sds, allows normal approximation to a binomial distribution
 * 	ensures both are eligible for proportion test
 */
bool twoProportionsSampleRequirements(double n1, double p1, double n2, double p2){
	return 	(n1 * p1 		< 10 || n1 * (1 - p1)	< 10) ||
			(n2 * p2 		< 10 || n2 * (1 - p2) 	< 10);
}

/**
 *
 */
double twoProportionsPPooledProportion(double recovered1, double n1, double recovered2, double n2){
	return (recovered1 + recovered2) / (n1 + n2);
}

/**
 *
 */
double twoProportionsUnpooledSE(double variance1, double n1, double variance2, double n2){
	return sqrt(
		(variance1/n1) + (variance2/n2)
	);
}

void calcConfidenceInterval(double pi, double pj,int i, int j, double SE, Matrix<string> CIMatrix){
	ostringstream oss0, oss1;
	oss0 << (pi - pj);
	oss1 << 1.96 * SE;
	CIMatrix.at(i, j) = oss0.str() + "±" + oss1.str();
	CIMatrix.at(j, i) = "-" + oss0.str() + "±" + oss1.str();
}


/**
 *	Calculates the PooledSE for the two proportions test.
 *	Applicable to Bernouilli/Binomial distributions.

 *	
 *	@param	n1					We use viable comparisons, because that is the most reliable metric for n1, do not use marker amount, oracleTotal or anything else.
 *	@param	n2
 *	@param	pooledProportion
 */
double twoProportionsPooledSE(double n1, double n2, double pooledProportion){
	return 	sqrt(
		pooledProportion * 
		(1 - pooledProportion) * 
		((1 / n1) + (1 / n2))
		); 
}


