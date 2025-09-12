#include "library/pattern/singleton.h"
#include "device.h"

namespace library {
	template<>
	d3d11::device* singleton<d3d11::device, true, true>::_instance = nullptr;
	template<>
	template<>
	declspec_dll auto singleton<d3d11::device, true, true>::construct<D3D_DRIVER_TYPE, unsigned int>(D3D_DRIVER_TYPE&& driver_type, unsigned int&& flag) noexcept -> d3d11::device& {
		_instance = library::allocate<d3d11::device>();
		::new(reinterpret_cast<void*>(_instance)) d3d11::device(driver_type, flag);
		return *_instance;
	}
	template<>
	declspec_dll auto singleton<d3d11::device, true, true>::instance(void) noexcept -> d3d11::device& {
		return *_instance;
	}
	template<>
	declspec_dll void singleton<d3d11::device, true, true>::destruct(void) noexcept {
		_instance->~device();
		library::deallocate(_instance);
	}
	template class declspec_dll singleton<d3d11::device, true, true>;
}

namespace d3d11 {

}