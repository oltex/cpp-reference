#pragma once
#pragma comment(lib,"dinput8.lib")
#include "design-pattern/singleton.h"

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class input_manager final : public singleton<input_manager> {
	friend class singleton<input_manager>;
	using byte = unsigned char;
private:
	inline explicit input_manager(void) noexcept {
		DirectInput8Create(/*inst*/, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<void**>(&_input), nullptr);

		_input->CreateDevice(GUID_SysMouse, &_mouse, nullptr);
		_mouse->SetDataFormat(&c_dfDIMouse);
		_mouse->SetCooperativeLevel(/*hwnd*/, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
		_mouse->Acquire();

		_input->CreateDevice(GUID_SysKeyboard, &_keyboard, nullptr);
		_keyboard->SetCooperativeLevel(/*hwnd*/, DISCL_BACKGROUND | DISCL_NONEXCLUSIVE);
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
public:
	inline void update(void) noexcept {
		_mouse->GetDeviceState(sizeof(_DIMOUSESTATE), &_mouse_state);
		_keyboard->GetDeviceState(256, _keyboard_state);


	}
public:
	inline bool key_press(void) const noexcept {
	}
	inline bool key_down(void) const noexcept {
	}
	inline bool key_up(void) const noexcept {
	}
private:
	IDirectInput8W* _input;
	IDirectInputDevice8W* _mouse;
	IDirectInputDevice8W* _keyboard;

	_DIMOUSESTATE _mouse_state;
	byte _keyboard_state[256];
};