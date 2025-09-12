#include "library/pattern/singleton.h"
#include "device.h"

namespace library {

	template<>
	d3d11::device* singleton< d3d11::device, true, true>::_instance = nullptr;
	//static type* _instance;

	//template<typename T, bool A, bool B>
	//T* singleton<T, A, B>::_instance = nullptr;
	//template<typename type>
	//template<typename... argument>
	//inline static auto construct(argument&&... arg) noexcept -> type& {

	//};

	template<>
	declspec_dll auto singleton<d3d11::device, true, true>::construct(void) noexcept -> d3d11::device& {
		_instance = library::allocate<d3d11::device>();
		library::construct(*_instance, D3D_DRIVER_TYPE_HARDWARE, D3D11_CREATE_DEVICE_DEBUG);
		return *_instance;
	}

	//template<>
	//template<>
	//auto singleton<d3d11::device, true, true>::construct<D3D_DRIVER_TYPE, unsigned int>(D3D_DRIVER_TYPE&& driver_type, unsigned int&& flag) noexcept -> d3d11::device& {
	//	_instance = library::allocate<d3d11::device>();
	//	library::construct(*_instance, driver_type, flag);
	//	return *_instance;
	//}

	//template<typename T, bool A, bool B>
	//T* singleton<T, A, B>::instance() {
	//	if (!_instance) {
	//		_instance = new T(); // 필요 시 커스텀 생성 로직
	//	}
	//	return _instance;
	//}

	//template auto declspec_dll singleton<d3d11::device, true, true>::construct<D3D_DRIVER_TYPE, unsigned int>(D3D_DRIVER_TYPE&&, unsigned int&&) noexcept -> d3d11::device&;
	template class declspec_dll singleton<d3d11::device, true, true>;

}
