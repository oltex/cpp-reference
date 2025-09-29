#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct buffer_descript : public D3D11_BUFFER_DESC {
		inline explicit buffer_descript(void) noexcept = default;
		inline explicit buffer_descript(unsigned int byte_width, D3D11_USAGE usage, unsigned int bind_flag, unsigned int cpu_access_flag, unsigned int misc_falg, unsigned int struct_byte_stride) noexcept {
			ByteWidth = byte_width;
			Usage = usage;
			BindFlags = bind_flag;
			CPUAccessFlags = cpu_access_flag;
			MiscFlags = misc_falg;
			StructureByteStride = struct_byte_stride;
		}
		inline explicit buffer_descript(buffer_descript const&) noexcept = default;
		inline explicit buffer_descript(buffer_descript&&) noexcept = default;
		inline auto operator=(buffer_descript const&) noexcept -> buffer_descript & = default;
		inline auto operator=(buffer_descript&&) noexcept -> buffer_descript & = default;
		inline ~buffer_descript(void) noexcept = default;
	};

	class declspec_dll buffer : public library::component<ID3D11Buffer> {
		using base = library::component<ID3D11Buffer>;
	public:
		inline explicit buffer(ID3D11Buffer* component = nullptr) noexcept;
		inline explicit buffer(buffer const&) noexcept = default;
		inline explicit buffer(buffer&&) noexcept = default;
		inline auto operator=(buffer const&) noexcept -> buffer & = default;
		inline auto operator=(buffer&&) noexcept -> buffer & = default;
		inline ~buffer(void) noexcept = default;
	};
}