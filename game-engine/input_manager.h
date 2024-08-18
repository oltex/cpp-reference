#pragma once
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "design-pattern/singleton.h"
#include "window/instance.h"
#include "window/window.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace engine {
	class input_manager final : public design_pattern::singleton<input_manager, design_pattern::member_static<input_manager>> {
		friend class design_pattern::singleton<input_manager, design_pattern::member_static<input_manager>>;
	public:
		enum class move : unsigned char {
			x, y, z
		};
		enum class button : unsigned char {
			left, right, wheel
		};
	public:
		inline static auto constructor(window::instance& instance, window::window& window) noexcept -> input_manager& {
			_instance = new input_manager(instance, window);
			atexit(destructor);
			return *_instance;
		}
	private:
 		inline explicit input_manager(window::instance& instance, window::window& window) noexcept {
			auto hinstance = instance.data();
			auto hwnd = window.data();
			DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&_input), nullptr);

			_input->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
			_mouse->SetDataFormat(&c_dfDIMouse);
			_mouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			_mouse->Acquire();

			_input->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);
			_keyboard->SetDataFormat(&c_dfDIKeyboard);
			_keyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			_keyboard->Acquire();
		};
		inline explicit input_manager(input_manager const& rhs) noexcept = delete;
		inline auto operator=(input_manager const& rhs) noexcept -> input_manager & = delete;
		inline explicit input_manager(input_manager&& rhs) noexcept = delete;
		inline auto operator=(input_manager&& rhs) noexcept -> input_manager & = delete;
		inline ~input_manager(void) noexcept {
			_keyboard->Unacquire();
			_keyboard->Release();
			_mouse->Unacquire();
			_mouse->Release();
			_input->Release();
		};
	public:
		inline void update(void) noexcept {
			_mouse->GetDeviceState(sizeof(_DIMOUSESTATE), &_mouse_state);
			_keyboard->GetDeviceState(256, _keyboard_state);
			memset(_keyboard_down_frame, 0, 256);
		}
	public:
		inline long mouse_move(move const move) const noexcept {
			return *(&_mouse_state.lX + static_cast<unsigned char>(move));
		}
		inline bool mouse_press(button const btn) const noexcept {
			return static_cast<bool>(_mouse_state.rgbButtons[static_cast<unsigned char>(btn)]);
		}
		//inline bool mouse_down(void) const noexcept {
		//}
		//inline bool mouse_up(void) const noexcept {
		//}
		inline bool key_press(unsigned char const key) const noexcept {
			return static_cast<bool>(_keyboard_state[key]);
		}
		inline bool key_down(unsigned char const key) noexcept {
			if (true == _keyboard_down_frame[key])
				return true;
			if (true == static_cast<bool>(_keyboard_state[key])) {
				if (false == _keyboard_down[key])
					return _keyboard_down_frame[key] = _keyboard_down[key] = true;
			}
			else
				_keyboard_down[key] = false;
			return false;
		}
		//inline bool key_up(unsigned char const key) const noexcept {
		//	switch (_keyboard_state[key]) {
		//	case true:
		//		break;
		//	case false:
		//		break;
		//	}
		//}
	private:
		IDirectInput8W* _input;
		IDirectInputDevice8W* _mouse;
		IDirectInputDevice8W* _keyboard;

		_DIMOUSESTATE _mouse_state;
		unsigned char _keyboard_state[256];

		bool _keyboard_down[256];
		bool _keyboard_down_frame[256];
	};
}