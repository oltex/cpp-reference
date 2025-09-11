#pragma once
#include "unknwn.h"
#include "../function.h"

namespace library {
	template<typename type = void>
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

		template<typename type>
		inline auto query_interface(void) noexcept -> type* {
			type* object;
			_component->QueryInterface<type>(&object);
			return object;
		}
		inline auto query_interface(IID id) noexcept -> void* {
			void* object;
			_component->QueryInterface(id, &object);
			return object;
		}
		inline auto data(void) noexcept -> type*& {
			return _component;
		}
	};
	template<>
	class component<void> {
		IUnknown* _unknown;
	public:
		inline explicit component(IUnknown* unknown) noexcept
			: _unknown(unknown) {
		};
		inline explicit component(component const&) noexcept = delete;
		inline explicit component(component&&) noexcept = delete;
		inline auto operator=(component const&) noexcept -> component & = delete;
		inline auto operator=(component&&) noexcept -> component & = delete;
		inline ~component(void) noexcept {
			if (nullptr != _unknown)
				_unknown->Release();
		};

		template<typename type>
		inline auto query_interface(void) noexcept -> type {
			type* object;
			_unknown->QueryInterface<type>(&object);
			return object;
		}
		inline auto query_interface(IID id) noexcept -> void* {
			void* object;
			_unknown->QueryInterface(id, &object);
			return object;
		}
	};
}