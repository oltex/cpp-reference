#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class declspec_dll buffer : public library::component<ID3D11Buffer>{
		using base = library::component<ID3D11Buffer>;
	public:
		inline explicit buffer(ID3D11Buffer* buffer = nullptr) noexcept;
		inline explicit buffer(buffer const&) noexcept = default;
		inline explicit buffer(buffer&&) noexcept = default;
		inline auto operator=(buffer const&) noexcept -> buffer & = default;
		inline auto operator=(buffer&&) noexcept -> buffer & = default;
		inline ~buffer(void) noexcept = default;
	};
}
//ID3D11Buffer