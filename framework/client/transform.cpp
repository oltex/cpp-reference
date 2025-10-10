#include "transform.h"

namespace framework {
	transform::transform(void) noexcept
		: component(component::type_id<transform>()) {
		_float4x4 = dmath::matrix::identity().store();
	};

	transform::transform(std::vector<double> translation, std::vector<double> rotation, std::vector<double> scale) noexcept
		: component(component::type_id<transform>()) {

		float tx = 0.0f, ty = 0.0f, tz = 0.0f;
		if (0 != translation.size()) {
			tx = static_cast<float>(translation[0]);
			ty = static_cast<float>(translation[1]);
			tz = static_cast<float>(translation[2]);
		}
		float rx = 0.0f, ry = 0.0f, rz = 0.0f, rw = 1.0f;
		if (0 != rotation.size()) {
			rx = static_cast<float>(rotation[0]);
			ry = static_cast<float>(rotation[1]);
			rz = static_cast<float>(rotation[2]);
			rw = static_cast<float>(rotation[3]);
		}

		float sx = 1.0f, sy = 1.0f, sz = 1.0f;
		if (0 != scale.size()) {
			sx = static_cast<float>(scale[0]);
			sy = static_cast<float>(scale[1]);
			sz = static_cast<float>(scale[2]);
		}

		auto scale2 = dmath::matrix::scaling(sx, sy, sz);
		auto rotate2 = dmath::vector::set(rx, ry, rz, rw).quaternion_normalize().rotate_quaternion();
		auto translation2 = dmath::matrix::translation(tx, ty, tz);
		_float4x4 = (scale2 * rotate2 * translation2).store();
	}
	void transform::set_parent(library::intrusive::share_pointer<transform, 0>& parent) noexcept {
		_parent = parent;
	}
	void transform::translate(dmath::float3 move) noexcept {
		using namespace dmath;
		auto matrix = _float4x4.load();

		auto position = vector(matrix.r[3]);
		auto right = vector(matrix.r[0]).set_w(0).normalize_3();
		auto up = vector(matrix.r[1]).set_w(0).normalize_3();
		auto forward = vector(matrix.r[2]).set_w(0).normalize_3();

		auto delta = vector::multiple_add(vector::replicate(move.x), right,
			vector::multiple_add(vector::replicate(move.y), up, forward.scale(move.z)));

		position = position.add(delta);
		matrix.r[3] = position.set_w(1);

		_float4x4 = matrix.store();
	}

}