#pragma once
#include "../../lockfree/pool.h"
#include "../../array.h"
#include <iostream>
#include <thread>
#include <vector>
#include <intrin.h>

namespace example::lockfree {
	inline static unsigned int __stdcall pool_function(void* arg) noexcept {
		library::lockfree::pool<int>& _pool = *reinterpret_cast<library::lockfree::pool<int>*>(arg);
		std::vector<int*> _vector;

		for (int loop = 0; loop < 1000; ++loop) {
			std::cout << "allocate" << std::endl;
			for (auto index = 0; index < 2; ++index) {
				int* value = _pool.allocate();
				*value = 10;
				_vector.emplace_back(value);
			}
			for (auto index = 0; index < 2; ++index) {
				if (11 != ++(*_vector[index]))
					__debugbreak();
			}
			std::cout << "deallocate" << std::endl;
			for (auto index = 0; index < 2; ++index) {
				int* value = _vector.back();
				_vector.pop_back();
				_pool.deallocate(value);
			}
		}
		return 0;
	}

	LARGE_INTEGER _frequency;
	inline static unsigned int __stdcall pool_performance(void* arg) noexcept {
		library::lockfree::pool<int>& _pool = *reinterpret_cast<library::lockfree::pool<int>*>(arg);
		library::array<int*, 500> _array;

		LARGE_INTEGER _start, _end;
		unsigned long long _sum = 0, _count = 0;
		for (;;) {
			QueryPerformanceCounter(&_start);
			for (int loop = 0; loop < 10000; ++loop) {
				for (auto index = 0; index < 500; ++index)
					_array[index] = _pool.allocate();
				for (auto index = 0; index < 500; ++index)
					_pool.deallocate(_array[index]);
			}
			QueryPerformanceCounter(&_end);
			_sum += _end.QuadPart - _start.QuadPart;
			_count++;
			printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
		}
		return 0;
	}

	inline void pool(void) noexcept {
		library::lockfree::pool<int> _pool;
		QueryPerformanceFrequency(&_frequency);

		int count;
		scanf_s("%d", &count);
		for (int i = 0; i < 4; ++i)
			(HANDLE)_beginthreadex(nullptr, 0, pool_performance, reinterpret_cast<void*>(&_pool), 0, 0);
		system("pause");
	}
}