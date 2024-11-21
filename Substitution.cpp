#include "Substitution.hpp"
#include <algorithm>
#include <random>
#include <assert.h>
//#include <iostream>

SP_net::SP_net(std::vector<uchar>& data, const size_t s_block_size, const size_t p_block_size,
			   std::vector<ul>&& s_seeds, std::vector<ul>&& p_seeds)
	: _s_size(s_block_size), _p_size(p_block_size),
	_s_seeds(s_seeds), _p_seeds(p_seeds)

{
	assert(data.size() % s_block_size == 0 && data.size() % p_block_size == 0, "Image size must be a multiple of the blocks amount");

	// transform pixels to byte rep
	_data.reserve(data.size() * 8);
	std::vector<uchar> byte;
	byte.reserve(8);

	for (uchar num : data) {

		uchar tmp = num;

		for (int i = 0; i < 8; i++) {
			byte.push_back(tmp & 1);
			tmp >>= 1;
		}

		std::reverse(byte.begin(), byte.end());
		_data.insert(_data.end(), byte.begin(), byte.end());
		byte.clear();
	}

	_table.resize(pow(2, _s_size));
}

void SP_net::substitute(ul seed) { 

	// creating substitution table
	std::vector<long> vec(pow(2, _s_size));

	for (long i = 0; i < pow(2, _s_size); i++) {
		vec[i] = i;
	}

	std::shuffle(vec.begin(), vec.end(), std::default_random_engine(seed));

	for (long i = 0; i < pow(2, _s_size); i++) {
		_table[i] = vec[i];
	}

	long tmp = 0;
	int k = 0;
	std::vector<uchar> transformed;
	transformed.reserve(_data.size());

	for (int i = 0; i < _data.size(); i++) {
		
		tmp = (tmp << 1) | _data[i];
		k++;

		if (k == _s_size) {

			long mapped = _table[tmp];

			for (int m = _s_size - 1; m >= 0; m--) {
				transformed.push_back((mapped >> m) & 1);
			}

			tmp = 0;
			k = 0;
		}
		
	}

	_data.swap(transformed);
}

void SP_net::permutate(ul seed) { 

	std::default_random_engine p = std::default_random_engine(seed);

	long tmp = 0;
	int k = 0;
	std::vector<uchar> transformed;
	transformed.reserve(_data.size());

	for (int i = 0; i < _data.size(); i++) {
		
		transformed.push_back(_data[i]);
		k++;

		if (k == _p_size) {
			std::shuffle(transformed.end() - _p_size, transformed.end(), p);
			k = 0;
		}
	}

	_data.swap(transformed);
}

std::vector<uchar> SP_net::transform(size_t round) {

	substitute(_s_seeds[round]);
	permutate(_p_seeds[round]);

	std::vector<uchar> image;
	image.reserve(_data.size() / 8);
	uchar tmp = 0;
	int i = 0;

	for (uchar num : _data) {
		
		tmp = (tmp << 1) | num; 
		i++;

		if (i == 8) {
			image.push_back(tmp);
			tmp = 0;
			i = 0; 
		}
	}

	return image;
}