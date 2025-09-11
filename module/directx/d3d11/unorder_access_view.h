#pragma once
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	class unorder_access_view : public library::component<ID3D11UnorderedAccessView> {
		using base = library::component<ID3D11UnorderedAccessView>;
	public:
		inline explicit unorder_access_view(ID3D11UnorderedAccessView* unorder_access_view) noexcept
			: base(unorder_access_view) {
		}
		inline explicit unorder_access_view(unorder_access_view const&) noexcept = default;
		inline explicit unorder_access_view(unorder_access_view&&) noexcept = default;
		inline auto operator=(unorder_access_view const&) noexcept -> unorder_access_view & = default;
		inline auto operator=(unorder_access_view&&) noexcept -> unorder_access_view & = default;
		inline ~unorder_access_view(void) noexcept = default;
	};
}