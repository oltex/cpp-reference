#pragma once
#include "module/window/instance.h"
#include "module/window/class.h"
#include "module/window/struct.h"

#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class client {
		d3d11::device& _device;
		dxgi::swap_chain _swap_chain;

		window::handle _handle;
	public:
		inline explicit client(void) noexcept
			: _device(library::singleton<d3d11::device, true, true>::construct(D3D_DRIVER_TYPE_HARDWARE, D3D11_CREATE_DEVICE_DEBUG)) {
			window::_class _class;
			_class.class_name(L"window");
			_class.style(CS_HREDRAW | CS_VREDRAW);
			_class.cursor(window::load_cursor(IDC_ARROW));
			_class.regist();
			window::_struct _struct;
			_struct.class_name(L"window");
			_struct.style(WS_OVERLAPPEDWINDOW);
			_struct.x(CW_USEDEFAULT);
			_struct.width(CW_USEDEFAULT);
			_struct.create_window(_handle);


			_handle.show(true);
			_handle.update();

			auto dxgi_device = _device.query_interface_dxgi_device();
			auto dxgi_adapter = dxgi_device.get_parent_adapter();
			auto dxgi_factory = dxgi_adapter.get_parent_factory();
			_swap_chain = dxgi_factory.create_swap_chain(_handle, 60, true);
			auto texture = _swap_chain.get_buffer_texture_2d();


		}
		inline explicit client(client const&) noexcept = delete;
		inline explicit client(client&&) noexcept = default;
		inline auto operator=(client const&) noexcept -> client & = delete;
		inline auto operator=(client&&) noexcept -> client & = default;
		inline ~client(void) noexcept {
			library::singleton<d3d11::device, true, true>::destruct();
		};

		inline void execute(void) noexcept {
			for (;;) {
				while (auto msg = window::peek_message()) {
					if (WM_QUIT == msg->message)
						return;
					else {
						window::translate_message(*msg);
						window::dispatch_message(*msg);
					}
				}
			}
		}
	};
}