#pragma once
#pragma comment(lib,"dinput8.lib")
#include "design-pattern/singleton.h"
#include "window/instance.h"
#include "window/window.h"
#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class input_manager final {
	enum state { down, press, up };
	enum button : unsigned char {
		left, right, wheel
	};
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
		_keyboard->SetCooperativeLevel(hwnd, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		_keyboard->Acquire();
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

	inline static input_manager& instance(window::instance instance, window::window window) noexcept {
		static input_manager inst(ptr);
		return inst;
	}
public:
	inline void update(void) noexcept {
		_mouse->GetDeviceState(sizeof(_DIMOUSESTATE), &_mouse_state);
		_keyboard->GetDeviceState(256, _keyboard_state);
	}
public:
	inline long mouse_move(void) const noexcept {
		return &_mouse_state.lX + eMouseMove);
	}

	inline bool mouse_press(button const btn) const noexcept {
		return static_cast<bool>(_mouse_state.rgbButtons[btn]);
	}
	inline bool mouse_down(void) const noexcept {
	}
	inline bool mouse_up(void) const noexcept {
	}

	inline bool key_press(unsigned char const key) const noexcept {
		return static_cast<bool>(_keyboard_state[key]);
	}
	inline bool key_down(unsigned char const key) noexcept {
		switch (_keyboard_state[key]) {
		case true:
			if (state::up == _keyboard_state2[key])
				_keyboard_state2[key] = down;
			break;
		case false:
			if (state::press == _keyboard_state2[key])
				_keyboard_state2[key] = up;
			break;
		}
		return false;
	}
	inline bool key_up(unsigned char const key) const noexcept {
		switch (_keyboard_state[key]) {
		case true:
			break;
		case false:
			break;
		}
	}
private:
	IDirectInput8W* _input;
	IDirectInputDevice8W* _mouse;
	IDirectInputDevice8W* _keyboard;

	_DIMOUSESTATE _mouse_state;
	unsigned char _keyboard_state[256];

	state _keyboard_state2[256];
};