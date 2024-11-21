#pragma once
#include <vector>

using uchar = unsigned char;
using uint = unsigned int;
using ul = unsigned long;

class SP_net {

public:
	SP_net() = delete;
	SP_net(std::vector<uchar>& data, const size_t s_block_size, const size_t p_block_size,
		std::vector<ul>&& s_seeds, std::vector<ul>&& p_seeds);

	/*void set_substitution_seed(unsigned long substitution_seed);
	void set_permutation_seed(unsigned long permutation_seed);*/

	std::vector<uchar> transform(size_t round);
private:
	void substitute(ul seed);
	void permutate(ul seed);

private:
	size_t _s_size;
	size_t _p_size;
	std::vector<uchar> _data;
	std::vector<ul> _table;
	std::vector<ul> _s_seeds;
	std::vector<ul> _p_seeds;
};