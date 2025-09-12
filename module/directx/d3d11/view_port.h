#pragma once
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>
#include <cassert>

namespace d3d11 {
	using view_port = D3D11_VIEWPORT;
	//class view_port : public D3D11_VIEWPORT {
	//	inline explicit view_port(float top_left_x, float top_left_y, float width, float height, float min_depth, float max_depth) noexcept {
	//		TopLeftX = top_left_x;
	//		TopLeftY = top_left_y;
	//		Width = width;
	//		Height = height;
	//		MinDepth = min_depth;
	//		MaxDepth = max_depth;
	//	}
	//	inline explicit view_port(view_port const&) noexcept = default;
	//	inline explicit view_port(view_port&&) noexcept = default;
	//	inline auto operator=(view_port const&) noexcept -> view_port & = default;
	//	inline auto operator=(view_port&&) noexcept -> view_port & = default;
	//	inline ~view_port(void) noexcept = default;
	//};
}