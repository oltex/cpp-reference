#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/vector.h"
#include "component.h"

namespace framework {

	class component_interfance {
	public:
		inline virtual ~component_interfance(void) = default;
	};
	template <typename type>
	class component_vector : public component_interfance, public library::vector<type> {
	public:
		inline virtual ~component_vector(void) = default;
	};

	class component_manager final {
		using size_type = unsigned int;
		library::unorder_map<size_type, component_interfance*> _component;
	public:
		inline explicit component_manager(void) noexcept = default;
		inline explicit component_manager(component_manager const&) noexcept = delete;
		inline explicit component_manager(component_manager&&) noexcept = delete;
		inline auto operator=(component_manager const&) noexcept -> component_manager & = delete;
		inline auto operator=(component_manager&&) noexcept -> component_manager & = delete;
		inline ~component_manager(void) noexcept = default;

		template<typename type>
		inline void add_component(void) noexcept {
			_component.emplace(component::type_id<type>(), new component_vector<type>);
			//auto result = _component.find(component::type_id<type>());
			//static_cast<component_vector<type>*>(result->_second)->emplace_back();
		}
		inline void get_component(void) noexcept {

		}
	};
}