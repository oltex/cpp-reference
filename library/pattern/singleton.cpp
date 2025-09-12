#include "singleton.h"

namespace library {
	template<>
	type* singleton<type, true, true>::_instance = nullptr;
	template<>
	template<>
	declspec_dll auto singleton<type, true, true>::construct<int>(int&& value) noexcept -> type& {
		_instance = library::allocate<type>();
		::new(reinterpret_cast<void*>(_instance)) type(value);
		return *_instance;
	}
	template<>
	declspec_dll auto singleton<type, true, true>::instance(void) noexcept -> type& {
		return *_instance;
	}
	template<>
	declspec_dll void singleton<type, true, true>::destruct(void) noexcept {
		_instance->~device();
		library::deallocate(_instance);
	}
	template class declspec_dll singleton<type, true, true>;
}