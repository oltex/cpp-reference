#pragma once
#include "design-pattern/singleton.h"
#include <chrono>
#include <thread>

namespace engine {
	class timer_manager final : public singleton<timer_manager> {
		friend class singleton<timer_manager>;
		friend class engine;
	private:
		inline explicit timer_manager(void) noexcept
			: _time(std::chrono::high_resolution_clock::now()) {
		}
		inline ~timer_manager(void) noexcept {
		}
	public:
		inline void update(void) noexcept {
			auto time = std::chrono::high_resolution_clock::now();
			_delta = std::chrono::duration_cast<std::chrono::nanoseconds>(time - _time);
			_time += _frame;

			if (_delta < _frame)
				std::this_thread::sleep_for(std::chrono::nanoseconds(_frame - _delta));
		}
	public:
		inline void set_frame(long long frame) noexcept {
			_frame = std::chrono::nanoseconds(long long(1000000000. / frame));
		}
		inline auto get_frame(void) const noexcept -> long long {
			return static_cast<long long>(1000000000. / _frame.count());
		}
		inline auto get_delta(void) const noexcept {
			return _delta;
		}
	private:
		std::chrono::high_resolution_clock::time_point _time;
		std::chrono::nanoseconds _frame;
		std::chrono::nanoseconds _delta;
	};
}