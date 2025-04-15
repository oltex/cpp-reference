#pragma once
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

namespace library::system::time::multimedia {
	inline static auto get_time(void) noexcept -> unsigned long {
		return timeGetTime();
	}
	inline static void begin_period(unsigned int peroid) noexcept {
		timeBeginPeriod(peroid);
	}
	inline static void end_period(unsigned int peroid) noexcept {
		timeEndPeriod(peroid);
	}
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