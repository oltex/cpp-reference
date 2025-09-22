#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/vector.h"
#include "library/pattern/singleton.h"
#include "component.h"

namespace framework {
	class component_vector {
	protected:
		using size_type = unsigned int;
	public:
		inline virtual ~component_vector(void) = default;

		inline virtual auto emplace(component* arg) noexcept -> component* = 0;
		inline virtual void reserve(size_type const capacity) noexcept = 0;
	};
	template <typename type>
	class component_vector_detail : public component_vector, public library::vector<type> {
		using base = library::vector<type>;
		using size_type = component_interfance::size_type;
	public:
		inline virtual ~component_vector(void) = default;

		inline virtual auto emplace(component* arg) noexcept -> component* override {
			return &base::emplace_back(arg);
		};
		inline virtual void reserve(size_type const capacity) noexcept override {
			base::reserve(capacity);
		}
	};

	class component_manager final {
		using size_type = unsigned int;
		library::unorder_map<size_type, component_vector*> _component;
	public:
		inline explicit component_manager(void) noexcept = default;
		inline explicit component_manager(component_manager const&) noexcept = delete;
		inline explicit component_manager(component_manager&&) noexcept = delete;
		inline auto operator=(component_manager const&) noexcept -> component_manager & = delete;
		inline auto operator=(component_manager&&) noexcept -> component_manager & = delete;
		inline ~component_manager(void) noexcept = default;

		template<typename type>
		inline auto create_component(void) noexcept -> type& {
			auto result = _component.find(component::type_id<type>());
			if (_component.end() == result) {
				result = _component.emplace(component::type_id<type>(), new component_vector<type>);
				result->_second->reserve(100);
			}
			return static_cast<component_vector<type>*>(result->_second)->emplace_back();
		}
		inline auto clone_component(framework::component* component) noexcept {
			auto result = _component.find(component->type_id());
			if (_component.end() == result)
				__debugbreak();
			result->_second->emplace(component);
			//return static_cast<component_vector<type>*>(result->_second)->emplace_back();
		}
	};
}