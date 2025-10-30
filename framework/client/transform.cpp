#include "transform.h"
#include "graphic.h"
#include "library/imgui/imgui.h"
#include "library/imgui/imgui_impl_dx11.h"
#include "library/imgui/imgui_impl_win32.h"
#include "library/imgui/imgui_internal.h"

namespace framework {
	transform::transform(void) noexcept
		: _float4x4(dmath::matrix::identity().store()), _quaternion(0.f, 0.f, 0.f, 1.f), _scale(1.f, 1.f, 1.f) {
	};
	transform::transform(std::vector<double> translation, std::vector<double> rotation, std::vector<double> scale) noexcept {
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
		auto rotate2 = dmath::vector(rx, ry, rz, rw).quaternion_normalize().matrix_rotate_quaternion();
		auto translation2 = dmath::matrix::translation(tx, ty, tz);
		_float4x4 = (scale2 * rotate2 * translation2).store();
	}

	void transform::edit(void) noexcept {
		if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_NoTreePushOnOpen)) {
			float position[3] { 0.f, 0.f, 0.f };
			float rotation[3] { 0.f, 0.f, 0.f };
			float scale[3] { 1.f, 1.f, 1.f };

			if (ImGui::BeginTable("##transform", 2, ImGuiTableFlags_Borders)) {
				ImGui::TableSetupColumn("##label", ImGuiTableColumnFlags_WidthStretch, 2.f);
				ImGui::TableSetupColumn("##value", ImGuiTableColumnFlags_WidthStretch, 8.f);
				constexpr char const* label[3]{ "Position", "Rotation", "Scale" };
				for (auto row = 0; row < 3; ++row) {
					ImGui::PushID(row);
					ImGui::TableNextRow();
					ImGui::TableNextColumn();
					ImGui::TextUnformatted(label[row]);
					ImGui::TableNextColumn();
					ImGui::SetNextItemWidth(-FLT_MIN);
					ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
					for (auto index = 0; index < 3; ++index) {
						ImGui::PushID(index);
						if (ImGui::DragFloat("##float", &position[index], 0.1f, -FLT_MAX, FLT_MAX, "%.3f")) {
						}
						ImVec2 min = ImGui::GetItemRectMin();
						ImVec2 max = ImGui::GetItemRectMax();
						const float bar_w = 3.0f;      // 라인 두께
						const float pad = 3.0f;      // 프레임 안쪽 여백
						constexpr ImU32 const color[3]{ IM_COL32(230,80,80,255), IM_COL32(80,200,120,255), IM_COL32(80,120,230,255) };
						ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(min.x + pad, min.y + pad), ImVec2(min.x + pad + bar_w, max.y - pad), color[index], 0.0f);
						ImGui::SameLine(0, ImGui::GetStyle().ItemInnerSpacing.x);
						ImGui::PopItemWidth();
						ImGui::PopID();
					}
					ImGui::PopID();
				}
				ImGui::EndTable();
			}
		}
	}
	auto transform::buffer(void) noexcept -> d3d11::buffer& {
		static d3d11::buffer _buffer = graphic::instance()._device.create_buffer(
			d3d11::buffer_descript(sizeof(dmath::float4x4), D3D11_USAGE_DYNAMIC, D3D11_BIND_CONSTANT_BUFFER, D3D11_CPU_ACCESS_WRITE, 0, 0), nullptr);
		return _buffer;
	}
	auto transform::matrix(void) noexcept -> dmath::matrix {
		return _float4x4;
	}

	void transform::translate(dmath::float3 move, bool local) noexcept {
		using namespace dmath;
		auto matrix = _float4x4.load();
		auto position = vector(matrix.r[3]);

		dmath::vector delta;
		if (true == local) {
			auto right = vector(matrix.r[0]).vector_normalize_3();
			auto up = vector(matrix.r[1]).vector_normalize_3();
			auto forward = vector(matrix.r[2]).vector_normalize_3();
			delta = vector::multiple_add(vector::replicate(move.x), right, vector::multiple_add(vector::replicate(move.y), up, forward.vector_scale(move.z)));
		}
		else
			delta = vector(move.x, move.y, move.z, 0);

		matrix.r[3] = position.vector_add(delta);
		_float4x4 = matrix.store();
	}
	void transform::rotate(dmath::float3 move, bool local) noexcept {
		using namespace dmath;
		auto quaternion = _quaternion.load();
		auto qx = vector::quaternion_rotate_axis(vector(1.f, 0.f, 0.f, 0.f), move.x);
		auto qy = vector::quaternion_rotate_axis(vector(0.f, 1.f, 0.f, 0.f), move.y);
		auto qz = vector::quaternion_rotate_axis(vector(0.f, 0.f, 1.f, 0.f), move.z);
		auto delta = qz.quaternion_multiple(qx).quaternion_multiple(qy);

		if (true == local)
			quaternion = delta.quaternion_multiple(quaternion).quaternion_normalize();
		else
			quaternion = quaternion.quaternion_multiple(delta).quaternion_normalize();
		_quaternion = quaternion;

		auto matrix = _float4x4.load();
		auto position = vector(matrix.r[3]);
		auto rotation = quaternion.matrix_rotate_quaternion();
		auto scale = _scale.load().matrix_scale();
		matrix = scale * rotation;
		matrix.r[3] = position;
		_float4x4 = matrix;
	}
}