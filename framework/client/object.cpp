#include "object.h"

namespace framework {
	object::object(void) noexcept {
	}
	object::object(object const& rhs) noexcept {
	}

	void objects::deallocate_object(object* value) noexcept {
		_pool.deallocate(value);
	}
	void objects::regist_prototype(library::string const& name, library::intrusive::share_pointer<object, 0>& object) noexcept {
		auto clone = allocate_object(*object);
		_prototype.emplace(name, clone);
	}
	auto objects::find_prototype(library::string const& name) noexcept -> library::intrusive::share_pointer<object, 0> {
		auto result = _prototype.find(name);
		return result->_second;
	}
}