#include "bound.h"

namespace dcoll {
	inline bound::bound(DirectX::BoundingBox bound_box) noexcept 
		: _bound(bound_box) {
	}
	inline bound::bound(DirectX::BoundingOrientedBox bound_oriented_box) noexcept 
		: _bound(bound_oriented_box) {
	}
	inline bound::bound(DirectX::BoundingSphere bound_sphere) noexcept 
		: _bound(bound_sphere) {
	}
	inline bound::bound(DirectX::BoundingFrustum bound_frustum) noexcept 
		: _bound(bound_frustum) {
	}

	inline void bound::transform(dmath::fmatrix matrix) noexcept {
		std::visit([&matrix](auto& shape) noexcept {
			using type = std::decay_t<decltype(shape)>;
			type shape2;
			shape.Transform(shape2, matrix);
			shape = shape2;
			}, _bound);
	}
	inline bool bound::Intersect(bound const& rhs) const noexcept {
		return std::visit([](auto&& bound1, auto&& bound2) noexcept -> bool {
			return bound1.Intersects(bound2);
			}, _bound, rhs._bound);
	}
	inline bool bound::contain(bound const& rhs) const noexcept {
		return std::visit([](auto&& bound1, auto&& bound2) noexcept -> bool {
			return bound1.Contains(bound2);
			}, _bound, rhs._bound);
	}
}