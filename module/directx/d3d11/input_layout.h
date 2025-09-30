#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3d11.lib")
#include <d3d11.h>

namespace d3d11 {
	struct input_element_descript : public D3D11_INPUT_ELEMENT_DESC {
		//inline explicit input_element_descript(void) noexcept = default;
		//inline explicit input_element_descript(input_element_descript const&) noexcept = default;
		//inline explicit input_element_descript(input_element_descript&&) noexcept = default;
		//inline auto operator=(input_element_descript const&) noexcept -> input_element_descript & = default;
		//inline auto operator=(input_element_descript&&) noexcept -> input_element_descript & = default;
		//inline ~input_element_descript(void) noexcept = default;
	};

	class declspec_dll input_layout : public library::component<ID3D11InputLayout> {
		using base = library::component<ID3D11InputLayout>;
	public:
		inline explicit input_layout(ID3D11InputLayout* component = nullptr) noexcept;
		inline explicit input_layout(input_layout const&) noexcept = default;
		inline explicit input_layout(input_layout&&) noexcept = default;
		inline auto operator=(input_layout const&) noexcept -> input_layout & = default;
		inline auto operator=(input_layout&&) noexcept -> input_layout & = default;
		inline ~input_layout(void) noexcept = default;
	};
}