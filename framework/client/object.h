#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
//#include "library/container/intrusive/list.h"
//#include "library/container/intrusive/share_pointer.h"
#include "component.h"

namespace framework {
	class object /*: public library::intrusive::list_hook<0>*/ {
		object* _parent;
		//library::intrusive::list<object, 0> _child;
		
		library::unorder_map<library::string, framework::component*> _component;
	public:
		inline explicit object(void) noexcept = default;
		inline explicit object(object const&) noexcept {
		};
		inline explicit object(object&&) noexcept = delete;
		inline auto operator=(object const&) noexcept -> object & = delete;
		inline auto operator=(object&&) noexcept -> object & = delete;
		inline ~object(void) noexcept = default;

		inline void add_component(library::string const& name, framework::component* component) noexcept {
			_component.emplace(name, component);
		}
	};

}