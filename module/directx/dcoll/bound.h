#pragma once
#include "library/define.h"
#include <DirectXCollision.h>
#include <variant>
#include "../dmath/matrix.h"

namespace dcoll {
	class declspec_dll bound {
#pragma warning(suppress: 4251)
		std::variant<DirectX::BoundingBox, DirectX::BoundingOrientedBox, DirectX::BoundingSphere, DirectX::BoundingFrustum> _bound;
	public:
		inline explicit bound(DirectX::BoundingBox bound_box) noexcept;
		inline explicit bound(DirectX::BoundingOrientedBox bound_oriented_box) noexcept;
		inline explicit bound(DirectX::BoundingSphere bound_sphere) noexcept;
		inline explicit bound(DirectX::BoundingFrustum bound_frustum) noexcept;
		inline explicit bound(bound const&) noexcept = default;
		inline explicit bound(bound&&) noexcept = default;
		inline auto operator=(bound const&) noexcept -> bound & = default;
		inline auto operator=(bound&&) noexcept -> bound & = default;
		inline ~bound(void) noexcept = default;

		inline void transform(dmath::fmatrix matrix) noexcept;
		inline bool Intersect(bound const& rhs) const noexcept;
		inline bool contain(bound const& rhs) const noexcept;
	};
}