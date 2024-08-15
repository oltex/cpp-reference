#pragma once
#include "design-pattern/singleton.h"

#include "object_manager.h"
#include "component_manager.h"
#include "frame_manager.h"

#pragma comment(lib, "Winmm.lib")
#include <Windows.h>

namespace engine {
	class engine final : public singleton<engine> {
		friend class singleton<engine>;
	private:
		inline explicit engine(void) noexcept
			: _object_manager(object_manager::instance()),
			_component_manager(component_manager::instance()),
			_frame_manager(frame_manager::instance()) {

			timeBeginPeriod(1);
		};
		inline ~engine(void) noexcept = default;
	public:
		inline void update(void) const noexcept {
			_frame_manager.set_frame(1);
			for (;;) {

				//test
				//static int _update_tick = 0;
				//static long _time = timeGetTime();
				//_update_tick++;
				//long time = timeGetTime();
				//if (time - _time >= 1000) {
				//	_time += 1000;
				//	_update_tick = 0;
				//}

				_frame_manager.update();
			}
		};
	private:
		object_manager& _object_manager;
		component_manager& _component_manager;
		frame_manager& _frame_manager;
	};
}