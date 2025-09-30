#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct unorder_access_view_descript : public D3D11_UNORDERED_ACCESS_VIEW_DESC {
	};
	class declspec_dll unorder_access_view : public library::component<ID3D11UnorderedAccessView> {
		using base = library::component<ID3D11UnorderedAccessView>;
	public:
		inline explicit unorder_access_view(ID3D11UnorderedAccessView* component = nullptr) noexcept;
		inline explicit unorder_access_view(unorder_access_view const&) noexcept = default;
		inline explicit unorder_access_view(unorder_access_view&&) noexcept = default;
		inline auto operator=(unorder_access_view const&) noexcept -> unorder_access_view & = default;
		inline auto operator=(unorder_access_view&&) noexcept -> unorder_access_view & = default;
		inline ~unorder_access_view(void) noexcept = default;
	};
}