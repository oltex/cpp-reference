#pragma once
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/vector.h"
#include "library/container/intrusive/list.h"
#include "object.h"
#include "system.h"

namespace framework {
	class scene {
		using size_type = unsigned int;
		library::unorder_map<library::string, library::vector<framework::object>> _object;
		library::unorder_map<size_type, library::vector<framework::system_interface*>> _system;
	public:
		inline explicit scene(void) noexcept = default;
		inline explicit scene(scene const&) noexcept = delete;
		inline explicit scene(scene&&) noexcept = delete;
		inline auto operator=(scene const&) noexcept -> scene & = delete;
		inline auto operator=(scene&&) noexcept -> scene & = delete;
		inline virtual ~scene(void) noexcept = default;

		inline void update(void) noexcept {
			for (auto& vector : _system)
				for (auto& system : vector._second)
					system->update();
		};
		inline auto add_object(void) noexcept {
		}
		inline auto find_object(library::string const& name) noexcept -> library::vector<framework::object>& {
			auto result = _object.find(name);
			return result->_second;
		}
		template <typename type>
		inline auto find_system(void) noexcept -> library::vector<framework::object*>& {
			auto result = _system.find(system::type_id<type>());
			return result->_second;
		}
	};
}