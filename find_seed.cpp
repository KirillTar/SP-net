#include "FInd_seed.hpp"
#include <iostream>
#include <queue>
#include <random>

using ul = unsigned long;
using uc = unsigned char;

struct Seed {
	float diff;
	ul seed;

	bool operator<(const Seed& rhs) {
		return diff < rhs.diff;
	}
};

bool operator < (const Seed& a, const Seed& b) {
	return a.diff < b.diff;
}

struct Max_4 {

	Max_4() {
		for (int i = 0; i < 4; i++) {
			arr[i] = 0.0;
		}
	}

	void push(Seed seed) {
		top.push(seed);

		if (top.size() > 4) {
			std::priority_queue<Seed> tmp;
			for (int i = 0; i < 4; i++) {
				tmp.push(top.top());
				arr[i] = top.top().diff;
				top.pop();
			}
			top.swap(tmp);
		}
		
	}

	bool better(long diff) {
		for (int i = 0; i < 4; i++) {
			if (diff > arr[i])
				return true;
		}
		return false;
	}

	std::priority_queue<Seed> top;
	float arr[4];
};


std::vector<uc> to_bits(const std::vector<uc>& vec) {
	std::vector<uc> data;
	data.reserve(data.size() * 4);
	std::vector<uc> byte;
	byte.reserve(4);

	for (uc num : vec) {

		uc tmp = num;

		for (int i = 0; i < 4; i++) {
			byte.push_back(tmp & 1);
			tmp >>= 1;
		}

		std::reverse(byte.begin(), byte.end());
		data.insert(data.end(), byte.begin(), byte.end());
		byte.clear();
	}

	return data;
}

float test_seed(ul seed, std::vector<uc>& bits, std::vector<uc>& vec) {

	std::vector<uc> table(vec.size());
	std::shuffle(vec.begin(), vec.end(), std::default_random_engine(seed));

	for (long i = 0; i < pow(2, 4); i++) {
		table[i] = vec[i];
	}

	table = to_bits(table);

	float sum = 0;
	int cur = 0;

	for (int i = 0; i < bits.size(); i++) {

		//std::cout << "curr: " << cur << '\n';

		if (i > 0 && i % 8 == 0) {

			if (cur < 2) {
				//std::cout << "diff less than 2" << '\n';
				return 0;
			}

			sum += cur;
			cur = 0;
		}


		if (bits[i] != table[i])
			cur++;
	}

	return sum / vec.size();
}

void find_seed() {

	std::vector<uc> vec(pow(2, 4));

	for (long i = 0; i < pow(2, 4); i++) {
		vec[i] = i;
	}

	std::vector<uc> bits = to_bits(vec);

	Max_4 max;

	for (int i = 0; i < 100000000; i++) {
		ul seed = rand() * rand();
		float diff = test_seed(seed, bits, vec);

		//std::cout << "diff: " << diff << " seed: " << seed << '\n';

		if (max.better(diff)) {
			max.push({ diff, seed });
		}

		if (i % 1'000'000 == 0)
			std::cout << i << '\n';
	}

	for (int i = 0; i < 4; i++) {
		std::cout << "diff: " << max.top.top().diff << " seed: " << max.top.top().seed << std::endl;
		max.top.pop();
	}
}