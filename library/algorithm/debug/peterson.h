#include <thread>
#include <iostream>
#include <Windows.h>
#include <intrin.h>

#include <intrin.h>
#include <Windows.h>
namespace debug {
	class peterson final {
	public:
		inline explicit peterson(void) noexcept = default;
		inline explicit peterson(peterson const& rhs) noexcept = delete;
		inline explicit peterson(peterson&& rhs) noexcept = delete;
		inline auto operator=(peterson const& rhs) noexcept -> peterson & = delete;
		inline auto operator=(peterson&& rhs) noexcept -> peterson & = delete;
		inline ~peterson() noexcept = default;
	public:
		//template<bool index>
		//inline void lock(void) noexcept {
		//	_flag[index] = true;
		//	_turn = index;
		//	MemoryBarrier();
		//	while (true == _flag[!index] && index == _turn) {
		//	}
		//};
		template<bool index>
		inline void lock(void) noexcept {
			_flag[0] = true;
			_turn = 0;
			bool flag;
			bool turn;
			//MemoryFence();
			MemoryBarrier();
			for (;;) {
				turn = _turn;
				flag = _flag[1];
				if (false == flag) {
					_reson[0] = 0;
					break;
				}
				else if (0 != turn) {
					_reson[0] = 1;
					break;
				}
			}
			_debug0_flag[0] = _flag[0];
			_debug0_flag[1] = flag;
			_debug0_turn = turn;
		};
		template<>
		inline void lock<true>(void) noexcept {
			_flag[1] = true;
			_turn = 1;
			bool flag;
			bool turn;
			//MemoryFence();
			MemoryBarrier();
			for (;;) {
				turn = _turn;
				flag = _flag[0];
				if (false == flag) {
					_reson[1] = 0;
					break;
				}
				else if (1 != turn) {
					_reson[1] = 1;
					break;
				}
			}
			_debug1_flag[0] = flag;
			_debug1_flag[1] = _flag[1];
			_debug1_turn = turn;
		};
		template<bool index>
		inline void unlock(void) noexcept {
			_flag[index] = false;
		}
	public:
		volatile bool _flag[2]{};
		volatile bool _turn;

		volatile bool _reson[2]{};
		volatile bool _debug0_flag[2]{};
		volatile bool _debug0_turn = 0;
		volatile bool _debug1_flag[2]{};
		volatile bool _debug1_turn = 0;
	};

	peterson _peter;
	unsigned long long _value = 0;
	long inter = 0;

	inline static unsigned int __stdcall peterson_function(void* arg) noexcept {
		for (int i = 0; i < 100000000; ++i) {
			_peter.lock<0>();
			//if (0x00 != InterlockedOr(&inter, 0x01))
			//	__debugbreak();
			_value++;
			//if (0x01 != InterlockedAnd(&inter, ~0x01))
			//	__debugbreak();
			_peter.unlock<0>();
		}
		return 0;
	}
	inline static unsigned int __stdcall peterson_function2(void* arg) noexcept {
		for (int i = 0; i < 100000000; ++i) {
			_peter.lock<1>();
			//if (0x00 != InterlockedOr(&inter, 0x02))
			//	__debugbreak();
			_value++;
			//if (0x02 != InterlockedAnd(&inter, ~0x02))
			//	__debugbreak();
			_peter.unlock<1>();
		}
		return 0;
	}

	inline void peterson_algorithm(void) noexcept {
		HANDLE handle[2];

		handle[0] = (HANDLE)_beginthreadex(nullptr, 0, peterson_function, nullptr, CREATE_SUSPENDED, 0);
		handle[1] = (HANDLE)_beginthreadex(nullptr, 0, peterson_function, nullptr, CREATE_SUSPENDED, 0);
		SetThreadAffinityMask(handle[0], 1 << 0);
		SetThreadAffinityMask(handle[1], 1 << 1);

		system("pause");
		auto rdtsc = __rdtsc();
		ResumeThread(handle[0]);
		ResumeThread(handle[1]);
		WaitForMultipleObjects(1, handle, true, INFINITE);
		std::cout << __rdtsc() - rdtsc << " : " << _value << std::endl;
		system("pause");
	}
}