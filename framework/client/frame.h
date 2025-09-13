#pragma once
#include "client.h"
#include "library/system/time.h"

namespace framework {
	class frame {
		unsigned long _time;
		unsigned long _frame = 1000 / 30;

		unsigned long _current_time;
		unsigned long _last_time;
	public:
		inline explicit frame(void) noexcept {
			library::time_begin_period(1);
			_time = library::time_get_time();
		};
		inline explicit frame(frame const&) noexcept = delete;
		inline explicit frame(frame&&) noexcept = delete;
		inline auto operator=(frame const&) noexcept -> frame & = delete;
		inline auto operator=(frame&&) noexcept -> frame & = delete;
		inline ~frame(void) noexcept {
			library::time_end_period(1);
		};

		inline auto update(void) noexcept {
			_current_time = library::time_get_time();
			auto delta = (float)(_current_time - _last_time) / 1000.f;
			_time += _frame;
			_last_time = _current_time;

			if (_time > _current_time) {
				auto a = _time - _current_time;
				Sleep(_time - _current_time);
			}
		}
	};
}