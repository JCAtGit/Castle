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
bool twoProportionsSampleRequirements(long double n1, long double p1, long double n2, long double p2){
	return 	(n1 * p1 		< 10 || n1 * (1 - p1)	< 10) ||
			(n2 * p2 		< 10 || n2 * (1 - p2) 	< 10);
}

/**
 *
 */
long double twoProportionsPPooledProportion(long double recovered1, long double n1, long double recovered2, long double n2){
	return (recovered1 + recovered2) / (n1 + n2);
}

/**
 *
 */
long double twoProportionsUnpooledSE(long double variance1, long double n1, long double variance2, long double n2){
	return sqrt(
		(variance1/n1) + (variance2/n2)
	);
}

void calcConfidenceInterval(long double pi, long double pj, int i, int j, long double SE, Matrix<string> &CIMatrix){
	ostringstream oss0, oss1;
	oss0 << (pi - pj);
	oss1 << 1.96 * SE;
	CIMatrix.at(i, j) = oss0.str() + " +/- " + oss1.str();
	CIMatrix.at(j, i) = "-" + oss0.str() + " +/- " + oss1.str();
}


/**
 *	Calculates the PooledSE for the two proportions test.
 *	Applicable to Bernouilli/Binomial distributions.

 *	
 *	@param	n1					We use viable comparisons, because that is the most reliable metric for n1, do not use marker amount, oracleTotal or anything else.
 *	@param	n2
 *	@param	pooledProportion
 */
long double twoProportionsPooledSE(long double n1, long double n2, long double pooledProportion){
	return 	sqrt(
		pooledProportion * 
		(1 - pooledProportion) * 
		((1 / n1) + (1 / n2))
		); 
}


