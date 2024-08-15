#pragma once
#include "design-pattern/singleton.h"

#include "object_manager.h"
#include "component_manager.h"
#include "timer_manager.h"

#pragma comment(lib, "Winmm.lib")
#include <Windows.h>

namespace engine {
	class engine final : public singleton<engine> {
		friend class singleton<engine>;
	private:
		inline explicit engine(void) noexcept
			: _object_manager(object_manager::instance()),
			_component_manager(component_manager::instance()),
			_timer_manager(timer_manager::instance()) {

			timeBeginPeriod(1);
		};
		inline ~engine(void) noexcept = default;
	public:
		inline void update(void) const noexcept {
			_timer_manager.set_frame(1);
			for (;;) {

				static int _update_tick = 0;
				static long _time = timeGetTime();
				_update_tick++;
				long time = timeGetTime();
				if (time - _time >= 1000) {
					_time += 1000;
					std::cout << _update_tick << std::endl;
					_update_tick = 0;
				}
				std::cout << _timer_manager.get_delta() << std::endl;

				_timer_manager.update();
			}
		};
	private:
		object_manager& _object_manager;
		component_manager& _component_manager;
		timer_manager& _timer_manager;
	};
}