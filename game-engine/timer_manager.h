#pragma once
#include "design-pattern/singleton.h"
#include "timer.h"

namespace engine {
	class timer_manager final : public singleton<timer_manager> {
		friend class singleton<timer_manager>;
		friend class engine;
	private:
		inline explicit timer_manager(void) noexcept {
		}
		inline ~timer_manager(void) noexcept {
		}
	public:
		inline void update(void) noexcept {
			auto time = std::chrono::high_resolution_clock::now();
			_delta.update(time);
			_frame.update(time);
			if (_frame.get_delta() < _frame.get_frame())
				std::this_thread::sleep_for(std::chrono::nanoseconds(_frame.get_frame() - _frame.get_delta()));
		}
	public:
		inline void set_frame(long long frame) noexcept {
			_frame.set_frame(std::chrono::nanoseconds(long long(1000000000. / frame)));
		}
		inline auto get_frame(void) const noexcept -> long long {
			return static_cast<long long>(1000000000. / _frame.get_frame().count());
		}
		inline auto get_delta(void) const noexcept -> float {
			return  std::chrono::duration_cast<std::chrono::duration<float>>(_delta.get_delta()).count();
		}
	private:
		timer _frame;
		timer _delta;
	};
}