#pragma once
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

namespace system_component::time {
	class multimedia final {
	public:
		inline static void begin_period(unsigned int peroid) noexcept {
			timeBeginPeriod(peroid);
		}
		inline static void end_period(unsigned int peroid) noexcept {
			timeEndPeriod(peroid);
		}
	public:
		inline explicit multimedia(void) noexcept = default;
		inline explicit multimedia(multimedia const& rhs) noexcept
			: _time(rhs._time) {
		};
		inline explicit multimedia(multimedia&& rhs) noexcept
			: _time(rhs._time) {
		}
		inline auto operator=(multimedia const& rhs) noexcept -> multimedia& {
			_time = rhs._time;
		}
		inline auto operator=(multimedia&& rhs) noexcept -> multimedia& {
			_time = rhs._time;
		}
		inline ~multimedia(void) noexcept = default;
	public:
		inline auto get_time(void) noexcept -> unsigned long {
			return _time = timeGetTime();
		}
	public:
		inline auto data(void) noexcept -> unsigned long& {
			return _time;
		}
	private:
		unsigned long _time;
	};

	//namespace period {
	//	//class raii final {
	//	//public:
	//	//	inline explicit raii(unsigned int peroid) noexcept
	//	//		: _peroid(peroid) {
	//	//		timeBeginPeriod(peroid);
	//	//	}
	//	//	inline ~raii(void) noexcept {
	//	//		timeEndPeriod(_peroid);
	//	//	}
	//	//private:
	//	//	unsigned int const _peroid;
	//	//};
	//}
}