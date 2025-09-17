#pragma once
#include "client.h"
#include "library/system/time.h"

namespace framework {
	class frame {
		double _frame = 1. / 3.;
		library::time _time;
		library::time _current, _prev;
	public:
		inline explicit frame(void) noexcept
			: _time(library::query_performance_count()), _prev(library::query_performance_count()) {
			library::time_begin_period(1);
		};
		inline explicit frame(frame const&) noexcept = delete;
		inline explicit frame(frame&&) noexcept = delete;
		inline auto operator=(frame const&) noexcept -> frame & = delete;
		inline auto operator=(frame&&) noexcept -> frame & = delete;
		inline ~frame(void) noexcept {
			library::time_end_period(1);
		};

		inline auto update(void) noexcept -> double {
			_current = library::query_performance_count();
			auto delta = _current - _prev;
			_prev = _current;
			return delta;
		}
		inline void sleep(void) noexcept {
			_time += _frame;
			if (_time > _current)
				library::sleep(static_cast<unsigned long>((_time - _current) * 1000.));
		}
	};
}
