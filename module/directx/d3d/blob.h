#pragma once
#include "library/define.h"
#include "library/system/component.h"
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

namespace d3d {
	class declspec_dll blob : public library::component<ID3DBlob> {
		using base = library::component<ID3DBlob>;
	public:
		inline explicit blob(ID3DBlob* blob) noexcept;
		inline explicit blob(blob const&) noexcept = default;
		inline explicit blob(blob&&) noexcept = default;
		inline auto operator=(blob const&) noexcept -> blob & = default;
		inline auto operator=(blob&&) noexcept -> blob & = default;
		inline ~blob(void) noexcept = default;

		inline auto get_buffer_pointer(void) noexcept -> void*;
		inline auto get_buffer_size(void) noexcept -> size_t;
	};

	inline auto complie_from_file(wchar_t const* const path, char const* const entry, char const* const target) noexcept -> blob {
		ID3DBlob* code;
#ifdef _DEBUG
		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
		unsigned int flag = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;
#endif
		D3DCompileFromFile(path, nullptr, D3D_COMPILE_STANDARD_FILE_INCLUDE, entry, target, flag, 0, &code, nullptr);

		return blob(code);
	}
}

