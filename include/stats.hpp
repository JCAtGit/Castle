#pragma once
#include <cmath>

using namespace std;

double standard_normal_cdf(double x) {
    return 1.0 - erfc(x / sqrt(2.0)) / 2.0;
}

bool twoProportionsSampleRequirements(double n1, double p1, double n2, double p2){
	return 	(n1 * p1 		< 10 || n1 * (1 - p1)	< 10) ||
			(n2 * p2 		< 10 || n2 * (1 - p2) 	< 10);
}

double twoProportionsPPooledProportion(double recovered1, double n1, double recovered2, double n2){
	return (recovered1 + recovered2) / (n1 + n2);
}

double twoProportionsUnpooledSE(double variance1, double n1, double variance2, double n2){
	return sqrt(
		(variance1/n1) + (variance2/n2)
	);
}


double twoProportionsPooledSE(double n1, double n2, double pooledProportion){
	return 	sqrt(
		pooledProportion * 
		(1 - pooledProportion) * 
		((1 / n1) + (1 / n2))
		); 
}