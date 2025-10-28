#pragma once
#include "library/pattern/singleton.h"
#include "library/system/thread.h"
#pragma comment(lib, "directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class graphic : public library::singleton<graphic> {
		friend class library::singleton<graphic>;
		friend class editors;
		friend class pipeline;
		friend class shader;
		friend class texture;
		friend class mesh;
		friend class menu;
		friend class camera;
		friend class transform;

		library::component<ID3D11Device> _device2;
		d3d11::device _device;
		d3d11::device_context _device_context;
		dxgi::swap_chain _swap_chain;
		d3d11::view_port _view_port;
		d3d11::render_target_view _render_target_view;

		//d3d11::view_port _view_port2;
		d3d11::render_target_view _render_target_view2;
		d3d11::shader_resource_view _shader_resource_view;
	public:
		explicit graphic(void) noexcept;
		explicit graphic(graphic const&) noexcept = delete;
		explicit graphic(graphic&&) noexcept = delete;
		auto operator=(graphic const&) noexcept -> graphic & = delete;
		auto operator=(graphic&&) noexcept -> graphic & = delete;
		~graphic(void) noexcept;

		void update(void) noexcept;
		void render(void) noexcept;
		void present(void) noexcept;
		auto device(void) noexcept -> d3d11::device&;
		auto device_context(void) noexcept -> d3d11::device_context&;
	};
}