#pragma once
#include "../../lockfree/queue.h"
//#include "../../my_class.h"
#include <thread>
#include <intrin.h>
#include <iostream>
#include <mutex>
#include <queue>
#include <Windows.h>
#include <set>
#include <list>


#pragma region push pop
//library::data_structure::lockfree::queue<unsigned long long> _lockfree_queue;
//inline static unsigned int __stdcall func_push(void* arg) noexcept {
//	int count = 0;
//	auto rdtsc = __rdtsc();
//	unsigned long long _value = 0;
//	for (int i = 0; i < 100000000; ++i) {
//		if (count == 1000000) {
//			rdtsc = __rdtsc() - rdtsc;
//			printf("thread_push : %d, %lld\n", GetCurrentThreadId(), rdtsc);
//			rdtsc = __rdtsc();
//			count = 0;
//		}
//		_lockfree_queue.emplace(_value++);
//		count++;
//	}
//	return 0;
//}
//
//inline static unsigned int __stdcall func_pop(void* arg) noexcept {
//	int count = 0;
//	auto rdtsc = __rdtsc();
//	unsigned long long _value = 0;
//	for (;;) {
//		if (count == 1000000) {
//			rdtsc = __rdtsc() - rdtsc;
//			printf("thread_pop  : %d, %lld\n", GetCurrentThreadId(), rdtsc);
//			rdtsc = __rdtsc();
//			count = 0;
//		}
//		//if (!_lockfree_queue.empty()) {
//		//	unsigned long long result = _lockfree_queue.pop();
//		//	//if (_value > result)
//		//	//	__debugbreak();
//		//	_value = result;
//		//	count++;
//		//}
//
//		auto result = _lockfree_queue.pop();
//		if (result) {
//			if (_value > result)
//				__debugbreak();
//			_value = *result;
//			count++;
//		}
//	}
//}
#pragma endregion

namespace example::lockfree {
	inline static unsigned int __stdcall queue_function(void* arg) noexcept {
		library::lockfree::queue<int>& _queue = *reinterpret_cast<library::lockfree::queue<int>*>(arg);
		int count = 0;
		for (;;) {
			if (count++ == 1000000) {
				printf("thread : %d\n", GetCurrentThreadId());
				count = 0;
			}
			for (auto index = 0; index < 2; ++index)
				_queue.emplace2(0);
			for (auto index = 0; index < 2; ++index)
				_queue.pop2();
		}
		return 0;
	}
	std::list<int> _result_list[5];
	unsigned int _result_list_id = 0;
	inline static unsigned int __stdcall queue_duplicate(void* arg) noexcept {
		library::lockfree::queue<int>& _queue = *(library::lockfree::queue<int>*)(arg);
		unsigned int _id = _InterlockedIncrement(&_result_list_id) - 1;
		static unsigned int _value = 0;
		for (auto loop = 0; loop < 100000; ++loop) {
			for (auto index = 0; index < 2; ++index)
				_queue.emplace2(_InterlockedIncrement(&_value));
			for (auto index = 0; index < 2; ++index) {
				if (auto result = _queue.pop2(); result)
					_result_list[_id].emplace_back(*result);
				else
					__debugbreak();
			}
		}
		printf("thread : %d\n", GetCurrentThreadId());
		return 0;
	}
	inline static unsigned int __stdcall queue_order(void* arg) noexcept {
		auto& _queue = *reinterpret_cast<library::lockfree::queue<std::pair<unsigned long, unsigned long long>>*>(arg);
		int count = 0;
		unsigned long long _value = 0;
		unsigned long long _prev_value = 0;
		for (;;) {
			if (count++ == 10000) {
				printf("thread : %d\n", GetCurrentThreadId());
				count = 0;
			}
			for (auto index = 0; index < 20; ++index)
				_queue.emplace2(std::pair<unsigned long, unsigned long long>(GetCurrentThreadId(), ++_value));
			for (auto index = 0; index < 20; ++index) {
				if (auto result = _queue.pop2(); result) {
					if (GetCurrentThreadId() == (*result).first) {
						if (_prev_value >= (*result).second)
							__debugbreak();
						else
							_prev_value = (*result).second;
					}
				}
				else
					__debugbreak();
			}
		}
		return 0;
	}
	inline static unsigned int __stdcall queue_share_pointer(void* arg) noexcept {
		library::lockfree::queue<std::shared_ptr<int>>& _queue = *reinterpret_cast<library::lockfree::queue<std::shared_ptr<int>>*>(arg);
		for (auto loop = 0; loop < 10000; ++loop) {
			for (auto index = 0; index < 50; ++index)
				_queue.emplace(std::make_shared<int>(0)/*new unsigned int(0)*/);
			for (auto index = 0; index < 50; ++index)
				auto result = _queue.pop();
		}
		printf("thread : %d\n", GetCurrentThreadId());
		return 0;
	}
	library::lockfree::queue<unsigned long long> _queue_array[5];
	unsigned int _lockfree_queue_index = 0;
	inline static unsigned int __stdcall queue_tls(void* arg) noexcept {
		unsigned int _id = _InterlockedIncrement(&_lockfree_queue_index) - 1;
		volatile unsigned int _prev_value = 0;
		volatile unsigned int _value = 0;
		int count = 0;
		for (;;) {
			if (count++ == 100000) {
				printf("thread : %d\n", GetCurrentThreadId());
				count = 0;
			}
			for (auto index = 0; index < 20; ++index)
				_queue_array[_id].emplace2(GetCurrentThreadId());
			for (auto index = 0; index < 20; ++index) {
				if (auto result = _queue_array[_id].pop2(); result) {
					if (GetCurrentThreadId() != (*result))
						__debugbreak();
				}
				else
					__debugbreak();
			}
		}
		return 0;
	}
#pragma endregion


	//alignas(64) SRWLOCK _srw;
	//alignas(64) CRITICAL_SECTION _cs;
	LARGE_INTEGER _frequency;
	inline static unsigned int __stdcall queue_performance(void* arg) noexcept {
		library::lockfree::queue<int>& _queue = *reinterpret_cast<library::lockfree::queue<int>*>(arg);
		LARGE_INTEGER _start, _end;
		long long _sum = 0, _count = 0;
		for (;;) {
			QueryPerformanceCounter(&_start);
			for (int index = 0; index < 10000; ++index) {
				for (auto push = 0; push < 500; ++push)
					_queue.emplace2(0);
				for (auto pop = 0; pop < 500; ++pop)
					_queue.pop2();
			}
			++_count;
			QueryPerformanceCounter(&_end);
			_sum += _end.QuadPart - _start.QuadPart;
			printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
		}
		return 0;
	}
	inline static unsigned int __stdcall queue_performance_srw(void* arg) noexcept {
		//LARGE_INTEGER _start;
		//LARGE_INTEGER _end;
		//long long _sum = 0;
		//long long _count = 0;
		//for (;;) {
		//	for (int i = 0; i < 100000; ++i) {
		//		for (auto j = 0; j < 500; ++j) {
		//			QueryPerformanceCounter(&_start);
		//			//EnterCriticalSection(&_cs);
		//			AcquireSRWLockExclusive(&_srw);
		//			_std_queue.push(0);
		//			ReleaseSRWLockExclusive(&_srw);
		//			//LeaveCriticalSection(&_cs);
		//			QueryPerformanceCounter(&_end);
		//			_sum += _end.QuadPart - _start.QuadPart;
		//			//for (volatile int k = 0; k < 64; ++k) {
		//			//}
		//		}
		//		for (auto j = 0; j < 500; ++j) {
		//			QueryPerformanceCounter(&_start);
		//			//EnterCriticalSection(&_cs);
		//			AcquireSRWLockExclusive(&_srw);
		//			_std_queue.pop();
		//			ReleaseSRWLockExclusive(&_srw);
		//			//LeaveCriticalSection(&_cs);
		//			QueryPerformanceCounter(&_end);
		//			_sum += _end.QuadPart - _start.QuadPart;
		//			//for (volatile int k = 0; k < 64; ++k) {
		//			//}
		//		}
		//	}
		//	++_count;
		//	printf("%f\n", (static_cast<double>(_sum) / _count) / static_cast<double>(_frequency.QuadPart) * 1e3);
		//}
		//return 0;
	}

	inline void queue(void) noexcept {
		library::lockfree::queue<int> _queue;
		//library::lockfree::queue<std::pair<unsigned long, unsigned long long>> _queue;
		//library::lockfree::queue<std::shared_ptr<int>> _queue;

		//QueryPerformanceFrequency(&_frequency);
		//InitializeSRWLock(&_srw);
		//InitializeCriticalSection(&_cs);

		int count;
		scanf_s("%d", &count);
		for (int i = 0; i < count; ++i)
			(HANDLE)_beginthreadex(nullptr, 0, queue_order, reinterpret_cast<void*>(&_queue), 0, 0);
		system("pause");


		std::set<int> _result_set;
		for (int i = 0; i < count; ++i) {
			for (auto& iter : _result_list[i]) {
				if (_result_set.count(iter))
					__debugbreak();
				_result_set.insert(iter);
			}
		}
		unsigned int expected = 1;
		for (auto val : _result_set) {
			if (val != expected)
				__debugbreak();
			++expected;
		}
		if (_result_set.size() != expected - 1)
			__debugbreak();
	}
}