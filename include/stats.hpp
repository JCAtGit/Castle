#pragma once
#include <cmath>

using namespace std;

double standard_normal_cdf(double x) {
    return erfc(-x / sqrt(2)) / 2;
}