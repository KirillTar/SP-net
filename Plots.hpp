#pragma once
#include "Tests.hpp"

namespace plt {
	void init();
	void close();
	using uchar_iter = std::vector<uchar>::const_iterator;
	void histogramm(std::vector<uchar>& data);
	void autocorrelation(std::vector<uchar>& data, uchar_iter begin_idx, uchar_iter end_idx);
	void lattice(std::vector<uchar>& data);
};