#include "graphic.h"
#include "window.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"

namespace framework {
	graphic::graphic(void) noexcept
		: _device(D3D_DRIVER_TYPE_HARDWARE, static_cast<unsigned int>(D3D11_CREATE_DEVICE_DEBUG)),
		_device_context(_device.get_immediate_context()),
		_view_port(0, 0, 1424, 720, 0.f, 1.f) {

		auto dxgi_device = _device.query_interface_dxgi_device();
		auto dxgi_adapter = dxgi_device.get_parent_adapter();
		auto dxgi_factory = dxgi_adapter.get_parent_factory();
		_swap_chain = dxgi_factory.create_swap_chain(_device, window::instance(), 60, true);

		{
			auto texture = _swap_chain.get_buffer_texture_2d();
			_render_target_view = _device.create_render_target_view(texture);
		}
		{
			auto texture = _device.create_texture_2d(d3d11::texture_2d_descript(1424, 720, 1, 1, DXGI_FORMAT_B8G8R8A8_UNORM, 1, 0, D3D11_USAGE_DEFAULT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE, 0, 0));
			_render_target_view2 = _device.create_render_target_view(texture);
			_shader_resource_view = _device.create_shader_resource_view(texture);
		}
	}
	graphic::~graphic(void) noexcept {
	}

	void graphic::update(void) noexcept {
		if (ImGui::Begin("Graphic", 0, ImGuiWindowFlags_MenuBar)) {
			if (ImGui::BeginMenuBar()) {
				static int rt_w = 1424, rt_h = 720;
				
				ImGui::TextUnformatted("Resolution"); 
				//ImGui::SetNextItemWidth(150);
				//ImGui::InputInt("##W", &rt_w, 0);
				//ImGui::TextUnformatted("x");
				//ImGui::SetNextItemWidth(150);
				//ImGui::InputInt("##W", &rt_w, 0);
				ImGui::EndMenuBar();
			}
			ImGui::Separator();
			ImVec2 avail = ImGui::GetContentRegionAvail();
			//float scale = ImMin(avail.x / tex_w, avail.y / tex_h);
			//ImVec2 size(tex_w * scale, tex_h * scale);

			ImGui::Image((ImTextureID)(_shader_resource_view.data()), avail, ImVec2(0, 0), ImVec2(1, 1));
		}
		ImGui::End();
	}

	void graphic::render(void) noexcept {
		auto rect = window::instance().get_client_rect();
		auto width = rect.right - rect.left;
		auto height = rect.bottom - rect.top;

		if ((width != 0 || height != 0) && (width != _view_port.Width || height != _view_port.Height)) {
			_device_context.set_render_target(0, nullptr, nullptr);
			_render_target_view = d3d11::render_target_view(nullptr);
			_swap_chain.resize_buffer(0, width, height, DXGI_FORMAT_UNKNOWN, 0);

			_view_port.Width = width;
			_view_port.Height = height;
			auto texture = _swap_chain.get_buffer_texture_2d();
			_render_target_view = _device.create_render_target_view(texture);
		}

		_device_context.set_view_port(_view_port);
		_device_context.set_render_target(1, &_render_target_view, nullptr);
		float color[4]{ 0.1f, 0.1f, 0.1f, 1.f };
		_device_context.clear_render_target_view(_render_target_view, color);

		_device_context.clear_render_target_view(_render_target_view2, color);

	}
	void graphic::present(void) noexcept {
		ImGui::Render();
		ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
		_swap_chain.present();
	}
	auto graphic::device(void) noexcept -> d3d11::device& {
		return _device;
	}
	auto graphic::device_context(void) noexcept -> d3d11::device_context& {
		return _device_context;
	}
}