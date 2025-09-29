#include "unorder_access_view.h"

namespace d3d11 {
	inline unorder_access_view::unorder_access_view(ID3D11UnorderedAccessView* component) noexcept
		: base(component) {
	}
}