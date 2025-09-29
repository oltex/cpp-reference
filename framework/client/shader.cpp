#include "shader.h"

namespace framework {
	shader::shader(wchar_t const* const path, char const* const entry, char const* const target) noexcept
		: _blob(d3d::complie_from_file(path, entry, target)) {
	}
}
