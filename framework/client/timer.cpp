#include "timer.h"

namespace framework {
	timer::timer(void) noexcept
		: _time(library::query_performance_count()), _prev(library::query_performance_count()) {
		library::time_begin_period(1);
	};
	timer::~timer(void) noexcept {
		library::time_end_period(1);
	};

	void timer::update_frame(void) noexcept {
		_current = library::query_performance_count();
		_delta = _current - _prev;
		_prev = _current;
	}
	void timer::sleep(void) noexcept {
		_time += _frame;
		if (_time > _current)
			library::sleep(static_cast<unsigned long>((_time - _current) * 1000.));
	}
}