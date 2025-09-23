#pragma once
#include "library/system/time.h"

namespace framework {
	class timer {
		double _frame = 1. / 3.;
		library::time _time;
		library::time _current, _prev;
		double _delta;
	public:
		explicit timer(void) noexcept;
		inline explicit timer(timer const&) noexcept = delete;
		inline explicit timer(timer&&) noexcept = delete;
		inline auto operator=(timer const&) noexcept -> timer & = delete;
		inline auto operator=(timer&&) noexcept -> timer & = delete;
		~timer(void) noexcept;

		void update_frame(void) noexcept;
		void sleep_frame(void) noexcept;
	};
}
