#include "resource.h"
#include "sound.h"

namespace framework {
	auto resources::find_resource(char const* const name) noexcept -> library::share_pointer<resource> {
		auto result = _resource.find(name);
		return result->_second;
	}
}
