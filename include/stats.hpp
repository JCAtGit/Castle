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
 *	@param n1 viable comparison within sample 1
 *	@param p1 recovered construct proportion of sample 1, i.e. recovered_construct / viable comparisons.
 *	@param n2 viable comparison within sample 2
 *	@param p2 recovered construct proportion of sample 2, idem 1.
 * 	@return True if requirement is not satisfied
 */
bool twoProportionsSampleRequirements(long double n1, long double p1, long double n2, long double p2){
	return 	(n1 * p1 		< 10 || n1 * (1 - p1)	< 10) ||
			(n2 * p2 		< 10 || n2 * (1 - p2) 	< 10);
}

/**
 * 	Calculates the pooled proportion value. 
 * 	@param recovered1	recovered_construct amount sample 1
 * 	@param n1			viable comparison within sample 1
 * 	@param recovered2	recovered_construct amount sample 2
 * 	@param n2			viable comparison within sample 2
 *	@return the pooled proportion value.
 */
long double twoProportionsPPooledProportion(long double recovered1, long double n1, long double recovered2, long double n2){
	return (recovered1 + recovered2) / (n1 + n2);
}

/**
 * 	Calculates the unpooled proportion value.
 *  @param variance1	variance sample 1
 *  @param n1			viable comparison within sample 1
 *  @param variance2	variance sample 2
 *  @param n2 			viable comparison within sample 2
 *	@return unpooled proportion standard error
 */
long double twoProportionsUnpooledSE(long double variance1, long double n1, long double variance2, long double n2){
	return sqrt(
		(variance1/n1) + (variance2/n2)
	);
}

/**
 *	Calculates the confidence interval for a pair i,j and records this within the CI-matrix.
 *	@param pi	recovery probability i
 *	@param pj	recovery probability i
 *	@param i	sample i
 *	@param j	sample j
 *	@param SE	standard error of pair i and j
 *	@param CIMatrix	Holds the calculated results of each pair
 *	@return CIMatrix filled with the confidence intervals
 */
void calcConfidenceInterval(long double pi, long double pj, int i, int j, long double SE, Matrix<string> &CIMatrix){
	ostringstream oss0, oss1;
	oss0 << (pi - pj);
	oss1 << 1.96 * SE; // Z_a / 2 = 1.96 voor alpha = 0.05
	CIMatrix.at(i, j) = oss0.str() + " +/- " + oss1.str();
	CIMatrix.at(j, i) = "-" + oss0.str() + " +/- " + oss1.str();
}

/**
 *	Calculates the PooledSE for the two proportions test.
 *	Applicable to Bernouilli/Binomial distributions.
 *	@param	n1					viable comparison within sample 1
 *	@param	n2					viable comparison within sample 2
 *	@param	pooledProportion	z-test pooled proportion value
 */
long double twoProportionsPooledSE(long double n1, long double n2, long double pooledProportion){
	return 	sqrt(
		pooledProportion * 
		(1 - pooledProportion) * 
		((1 / n1) + (1 / n2))
		); 
}


