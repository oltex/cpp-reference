#pragma once
#include "../../design-pettern/singleton/singleton.h"
#include "../../system/window/window.h"
#include "../../system/window/device_context.h"
#include <windowsx.h>

#include "camera.h"
#include "tree.h"
#include <string>

#include "tree/set.h"


class client final : public library::design_pattern::singleton<client, library::design_pattern::member_static<client>> {
private:
	friend class library::design_pattern::singleton<client, library::design_pattern::member_static<client>>;
	using size_type = unsigned int;
public:
	inline static auto constructor(window::window&& window) noexcept -> client& {
		_instance = new client(std::move(window));
		atexit(destructor);
		return *_instance;
	}
private:
	inline explicit client(window::window&& window) noexcept
		: _window(std::move(window)),
		_dc(_window.get_device_context().create_compatible_device_context()),
		_bitmap(_window.get_device_context().create_compatible_bitmap(_window.get_client_rect().right, _window.get_client_rect().bottom)),
		_background(RGB(30, 30, 30)) {

		_dc.select_object(_bitmap);
		RECT rect = _window.get_client_rect();
		_camera.set_position(POINT{ rect.right / 2, rect.bottom / 2 });
		_camera.set_rect(POINT{ rect.right, rect.bottom });
	}
	inline explicit client(client const& rhs) noexcept = delete;
	inline explicit client(client&& rhs) noexcept = delete;
	inline auto operator=(client const& rhs) noexcept -> client & = delete;
	inline auto operator=(client&& rhs) noexcept -> client & = delete;
	inline ~client(void) noexcept = default;
public:
	inline void initialize(void) const noexcept {
		_window.show(true);
		_window.update();
	}
	inline void procedure(HWND const hwnd, UINT const message, WPARAM const wparam, LPARAM const lparam) noexcept {
		switch (message) {
		case WM_KEYDOWN: {
			if (48 <= wparam && 57 >= wparam)
				_input += (int)wparam;
			else if (VK_RETURN == wparam) {
				if (_input.empty())
					break;
				int index = std::stoi(_input.c_str());
				_set.emplace(index);
				_input.clear();
			}
			else if (VK_SHIFT == wparam) {
				if (_input.empty())
					break;
				int index = std::stoi(_input.c_str());
				//_map.erase(index);
				_input.clear();
			}
			else if (VK_BACK == wparam) {
				if (_input.empty())
					break;
				_input.pop_back();
			}
			if (0x51 == wparam) {
				//_test.onoff(_window);
			}
			_window.invalidate_rect(nullptr, true);
		} break;
		case WM_TIMER: {
			if (1 != wparam)
				break;
			//_test.run(_map);
			_window.invalidate_rect(nullptr, true);
		} break;
		case WM_MOUSEWHEEL: {
			int delta = GET_WHEEL_DELTA_WPARAM(wparam);
			POINT point;
			GetCursorPos(&point);
			_window.screen_to_client(&point);
			if (delta > 0)
				_camera.zoom(point, 0.1f);
			else
				_camera.zoom(point, -0.1f);
			_window.invalidate_rect(nullptr, true);
		} break;
		case WM_PAINT: {
			RECT rect = _window.get_client_rect();
			_dc.fill_rect(&rect, _background);

			_dc.select_object(_tree._line);
			auto iter = _set.begin();
			for (unsigned int i = 0; i < _set.size(); ++i, ++iter) {
				RECT ellipse{ LONG(i * _tree._ellipse_size), LONG(iter._depth * _tree._ellipse_size), LONG((i + 1) * _tree._ellipse_size), LONG((iter._depth + 1) * _tree._ellipse_size) };
				iter._node->_position = POINT{ (ellipse.left + ellipse.right) / 2, (ellipse.top + ellipse.bottom) / 2 };

				POINT position;
				if (false == iter._node->_child[direction::left]->_nil) {
					position = _camera.clinet_to_camrea(iter._node->_position);
					_dc.move_to(position.x, position.y);
					position = _camera.clinet_to_camrea(iter._node->_child[direction::left]->_position);
					_dc.line_to(position.x, position.y);
				}
				if (false == iter._node->_parent->_nil && iter._node->_parent->_child[direction::right] == iter._node) {
					position = _camera.clinet_to_camrea(iter._node->_position);
					_dc.move_to(position.x, position.y);
					position = _camera.clinet_to_camrea(iter._node->_parent->_position);
					_dc.line_to(position.x, position.y);
				}
			}

			_dc.select_object(_tree._font);
			_dc.select_object(GetStockObject(NULL_PEN));
			_dc.set_bk_mode(TRANSPARENT);
			_dc.set_text_color(RGB(255, 255, 255));
			auto iter2 = _set.begin();
			for (unsigned int i = 0; i < _set.size(); ++i, ++iter2) {
				RECT ellipse{ LONG(i * _tree._ellipse_size), LONG(iter2._depth * _tree._ellipse_size), LONG((i + 1) * _tree._ellipse_size), LONG((iter2._depth + 1) * _tree._ellipse_size) };
				ellipse = _camera.clinet_to_camrea(ellipse);

				if (iter2._node->_color == color::red)
					_dc.select_object(_tree._red);
				else
					_dc.select_object(_tree._black);
				_dc.ellipse(ellipse.left, ellipse.top, ellipse.right, ellipse.bottom);
				std::wstring wstr = std::to_wstring(*iter2);
				_dc.draw_text(wstr.c_str(), (int)wstr.size(), &ellipse, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			window::device_context dc = _window.begin_paint();
			dc.bit_blt(0, 0, rect.right, rect.bottom, _dc, 0, 0, SRCCOPY);
			_window.end_paint(dc);
		} break;
		}
	}
public:
	window::window _window;
	window::device_context _dc;
	window::bitmap _bitmap;
	window::brush _background;

	camera _camera;
	tree _tree;
	std::string _input;

	set<int> _set;
};