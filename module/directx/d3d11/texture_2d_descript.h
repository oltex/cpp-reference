#pragma once
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct texture_2d_descript : public D3D11_TEXTURE2D_DESC {
		inline explicit texture_2d_descript(unsigned int width, unsigned int height, unsigned int mip_level, unsigned int array_size,
			DXGI_FORMAT format, unsigned int sample_descript_count, unsigned int sample_descript_quality, D3D11_USAGE usage,
			unsigned int bind_flag, unsigned int cpu_access_flag, unsigned int misc_flag, D3D11_SUBRESOURCE_DATA* data = nullptr) noexcept {
			Width = width;
			Height = height;
			MipLevels = mip_level;
			ArraySize = array_size;
			Format = format;
			SampleDesc.Count = sample_descript_count;
			SampleDesc.Quality = sample_descript_quality;
			Usage = usage;
			BindFlags = bind_flag;
			CPUAccessFlags = cpu_access_flag;
			MiscFlags = misc_flag;
		}
		inline explicit texture_2d_descript(texture_2d_descript const&) noexcept = default;
		inline explicit texture_2d_descript(texture_2d_descript&&) noexcept = default;
		inline auto operator=(texture_2d_descript const&) noexcept -> texture_2d_descript & = default;
		inline auto operator=(texture_2d_descript&&) noexcept -> texture_2d_descript & = default;
		inline ~texture_2d_descript(void) noexcept = default;
	};

}
