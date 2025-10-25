#pragma once
#include "unknwn.h"
#include "../function.h"
#include <cassert>

namespace library {
	inline void component_initialize(COINIT const coinit) noexcept {
		auto result = ::CoInitializeEx(nullptr, coinit);
	}
	inline void component_uninitialize(void) noexcept {
		::CoUninitialize();
	}
	template<typename type>
	inline auto component_create_instance(IID const& cls_id, unsigned long cls_constext = CLSCTX_INPROC_SERVER) noexcept -> type* {
		type* component;
		auto result = ::CoCreateInstance(cls_id, nullptr, cls_constext, __uuidof(type), reinterpret_cast<void**>(&component));
		return component;
	}
	inline auto component_task_memory_free(void* pointer) noexcept {
		::CoTaskMemFree(pointer);
	}

	template<typename type>
	class component {
	protected:
		type* _component;
	public:
		inline explicit component(type* component = nullptr) noexcept
			: _component(component) {
		};
		inline explicit component(component const& rhs) noexcept
			: _component(rhs._component) {
			if (nullptr != _component)
				_component->AddRef();
		};
		inline explicit component(component&& rhs) noexcept
			: _component(library::exchange(rhs._component, nullptr)) {
		};
		inline auto operator=(component const& rhs) noexcept -> component& {
			if (nullptr != _component)
				_component->Release();
			_component = rhs._component;
			if (nullptr != _component)
				_component->AddRef();
			return *this;
		};
		inline auto operator=(component&& rhs) noexcept -> component& {
			if (nullptr != _component)
				_component->Release();
			_component = library::exchange(rhs._component, nullptr);
			return *this;
		};
		inline ~component(void) noexcept {
			if (nullptr != _component)
				_component->Release();
		};

		inline bool operator==(std::nullptr_t) noexcept {
			return nullptr == _component;
		}
		inline auto operator*(void) noexcept -> type& {
			return *_component;
		}
		inline auto operator->(void) noexcept -> type* {
			return _component;
		}
		inline operator type* (void) const noexcept {
			return _component;
		}
		template<typename type>
		inline auto query_interface(void) noexcept -> component<type> {
			type* object;
			auto result = _component->QueryInterface<type>(&object);
			assert(SUCCEEDED(result));
			return component<type>(object);
		}
		inline auto query_interface(IID id) noexcept -> void* {
			void* object;
			auto result = _component->QueryInterface(id, &object);
			assert(SUCCEEDED(result));
			return object;
		}
		inline auto data(void) noexcept -> type*& {
			return _component;
		}
	};
}