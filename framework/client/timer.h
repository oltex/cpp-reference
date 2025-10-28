#pragma once
#include "library/system/time.h"

namespace framework {
	class timer {
		double _frame = 1. / 60.;
		library::time _time;
		library::time _current, _prev;
		double _delta;
	public:
		explicit timer(void) noexcept;
		explicit timer(timer const&) noexcept = delete;
		explicit timer(timer&&) noexcept = delete;
		auto operator=(timer const&) noexcept -> timer & = delete;
		auto operator=(timer&&) noexcept -> timer & = delete;
		~timer(void) noexcept;

		void update_frame(void) noexcept;
		void sleep(void) noexcept;
	};
}
