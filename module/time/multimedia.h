#pragma once
#include <Windows.h>
#pragma comment(lib, "Winmm.lib")

namespace timer {
	namespace multimedia {
		class period final {
		public:
			inline explicit period(unsigned int peroid) noexcept 
				: _peroid(peroid) {
				timeBeginPeriod(peroid);
			}
			inline ~period(void) noexcept {
				timeEndPeriod(_peroid);
			}
		public:
			inline static void begin(unsigned int peroid) noexcept {
				timeBeginPeriod(peroid);
			}
			inline static void end(unsigned int peroid) noexcept {
				timeEndPeriod(peroid);
			}
		private:
			unsigned int const _peroid;
		};
		inline static auto get_time(void) noexcept -> unsigned long {
			return timeGetTime();
		}
	}
}