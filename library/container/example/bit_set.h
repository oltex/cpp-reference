#pragma once
#include "../bit_set.h"
#include <bitset>

namespace example {
	inline void bit_set(void) noexcept {
		std::bitset<255> std_bitset;
		std_bitset[55] = 1;
		library::bit_set<64> bitset;
		library::bit_set<64> bitset2;
		bitset.set(55, 1);
		auto b = bitset.test(55);

		bitset2.set(5, 1);
		auto bit3 = bitset | bitset2;

	}
}