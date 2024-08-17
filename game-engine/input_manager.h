#pragma once
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#include "window/instance.h"
#include "window/window.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

namespace engine {
	class input_manager final {
		enum state { down, press, up };
		enum class move : unsigned char {
			x, y, z
		};
		enum button : unsigned char {
			left, right, wheel
		};
	private:
		inline explicit input_manager(window::instance& instance, window::window& window) noexcept {
			HRESULT hr = S_OK;
			auto hinstance = instance.data();
			auto hwnd = window.data();
			hr |= DirectInput8Create(hinstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&_input), nullptr);

			hr |= _input->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
			hr |= _mouse->SetDataFormat(&c_dfDIMouse);
			hr |= _mouse->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			hr |= _mouse->Acquire();

			hr |= _input->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);
			hr |= _keyboard->SetDataFormat(&c_dfDIKeyboard);
			hr |= _keyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
			hr |= _keyboard->Acquire();
		};
		inline ~input_manager(void) noexcept {
			_keyboard->Unacquire();
			_keyboard->Release();
			_mouse->Unacquire();
			_mouse->Release();
			_input->Release();
		};
		inline explicit input_manager(input_manager const& rhs) noexcept = delete;
		inline auto operator=(input_manager const& rhs) noexcept -> input_manager & = delete;
		inline explicit input_manager(input_manager&& rhs) noexcept = delete;
		inline auto operator=(input_manager&& rhs) noexcept -> input_manager & = delete;
	public:
		inline static input_manager& instance(window::instance* instance = nullptr, window::window* window = nullptr) noexcept {
			static input_manager inst(*instance, *window);
			return inst;
		}
	public:
		inline void update(void) noexcept {
			_mouse->GetDeviceState(sizeof(_DIMOUSESTATE), &_mouse_state);
			_keyboard->GetDeviceState(256, _keyboard_state);
		}
	public:
		inline long mouse_move(move const move) const noexcept {
			return *(&_mouse_state.lX + static_cast<unsigned char>(move));
		}
		inline bool mouse_press(button const btn) const noexcept {
			return static_cast<bool>(_mouse_state.rgbButtons[btn]);
		}
		//inline bool mouse_down(void) const noexcept {
		//}
		//inline bool mouse_up(void) const noexcept {
		//}
		inline bool key_press(unsigned char const key) const noexcept {
			return static_cast<bool>(_keyboard_state[key]);
		}
		//inline bool key_down(unsigned char const key) noexcept {
		//	switch (_keyboard_state[key]) {
		//	case true:
		//		if (state::up == _keyboard_state2[key])
		//			_keyboard_state2[key] = down;
		//		break;
		//	case false:
		//		if (state::press == _keyboard_state2[key])
		//			_keyboard_state2[key] = up;
		//		break;
		//	}
		//	return false;
		//}
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

		state _keyboard_state2[256];
	};
}