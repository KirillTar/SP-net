#pragma once
#include <vector>
#include <numeric>

using uchar = unsigned char;
std::vector<double> autocorrelation_test(const std::vector<uchar>& data);
std::vector<double> histogramm_test(const std::vector<uchar>& data);


