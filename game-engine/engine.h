#pragma once
#include "design-pattern/singleton.h"
#include "window/instance.h"
#include "window/window.h"

#include "object_manager.h"
#include "component_manager.h"
#include "timer_manager.h"
#include "input_manager.h"

//test
//#pragma comment(lib, "Winmm.lib")
//#include <Windows.h>
#include <iostream>

namespace engine {
	class engine final : public design_pattern::singleton<engine, design_pattern::member_static<engine>> {
		friend class design_pattern::singleton<engine, design_pattern::member_static<engine>>;
	public:
		inline static auto constructor(window::instance& instance, window::window& window) noexcept -> engine& {
			_instance = new engine(instance, window);
			atexit(destructor);
			return *_instance;
		}
	private:
		inline explicit engine(window::instance& instance, window::window& window) noexcept
			: _object_manager(object_manager::instance()),
			_component_manager(component_manager::instance()),
			_timer_manager(timer_manager::instance()),
			_input_manager(input_manager::constructor(instance, window)) {
			//timeBeginPeriod(1);
		};
		inline explicit engine(engine const& rhs) noexcept = delete;
		inline auto operator=(engine const& rhs) noexcept -> engine & = delete;
		inline explicit engine(engine&& rhs) noexcept = delete;
		inline auto operator=(engine&& rhs) noexcept -> engine & = delete;
		inline ~engine(void) noexcept = default;
	public:
		inline virtual void Initialize(void) noexcept {
		}
		inline void update(void) const noexcept {
			_timer_manager.set_frame(50);

			MSG msg;
			for (;;) {
				if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
					if (WM_QUIT == msg.message)
						break;
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				_input_manager.update();

				if (_input_manager.key_down(DIK_E))
					std::cout << "press" << std::endl;
				//if(_input_manager.mouse_press(input_manager::button::wheel))
				//	std::cout << "press" << std::endl;

				//static int _update_tick = 0;
				//static long _time = timeGetTime();
				//_update_tick++;
				//long time = timeGetTime();
				//if (time - _time >= 1000) {
				//	_time += 1000;
				//	std::cout << _update_tick << std::endl;
				//	_update_tick = 0;
				//}
				//std::cout << _timer_manager.get_delta() << std::endl;

				_timer_manager.update();
			}

		};
	private:
		object_manager& _object_manager;
		component_manager& _component_manager;
		timer_manager& _timer_manager;
		input_manager& _input_manager;
	};
}