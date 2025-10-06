#pragma once
#include "library/pattern/singleton.h"
#include "library/system/thread.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
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

		d3d11::device _device;
		d3d11::device_context _device_context;
		dxgi::swap_chain _swap_chain;
	public:
		explicit graphic(void) noexcept;
		explicit graphic(graphic const&) noexcept = delete;
		explicit graphic(graphic&&) noexcept = delete;
		auto operator=(graphic const&) noexcept -> graphic & = delete;
		auto operator=(graphic&&) noexcept -> graphic & = delete;
		~graphic(void) noexcept;

		void render_ready(void) noexcept;
		void render_start(void) noexcept;
	};
}