#pragma once
#include <cmath>

using namespace std;

double standard_normal_cdf(double x) {
    return 1.0 - erfc(x / sqrt(2.0)) / 2.0;
}