#pragma once
#include "../unorder_set.h"
#include "my_class.h"

#include <unordered_set>
#include <intrin.h>
#include <iostream>
#include <Windows.h>

namespace example {
	inline void unorder_set(void) noexcept {
		library::unorder_set<int> set;
		std::unordered_set<int> std_set;

		for (int i = 0; i < 10000; ++i)
			set.emplace(i);
		set.clear();
		for (int i = 0; i < 10000; ++i)
			set.emplace(i);
		for (int i = 0; i < 10000; ++i)
			std_set.emplace(i);
		std_set.clear();
		for (int i = 0; i < 10000; ++i)
			std_set.emplace(i);

		if (set.bucket_count() != std_set.bucket_count())
			__debugbreak();

		for (int i = 0; i < set.bucket_count(); ++i) {
			auto set_iter = set.begin(i);
			auto std_set_iter = std_set.begin(i);

			while (set_iter != set.end(i)) {
				if (*set_iter != *std_set_iter)
					__debugbreak();
				++set_iter;
				++std_set_iter;
			}
		}
	}
}