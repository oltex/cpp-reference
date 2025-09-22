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
		object_list _object_list;
		library::unorder_map<library::string, object_weak_ptr> _object_tag;
		library::unorder_map<size_type, library::vector<framework::system_interface*>> _system;
	public:
		explicit scene(void) noexcept;
		inline explicit scene(scene const&) noexcept = delete;
		inline explicit scene(scene&&) noexcept = delete;
		inline auto operator=(scene const&) noexcept -> scene & = delete;
		inline auto operator=(scene&&) noexcept -> scene & = delete;
		inline ~scene(void) noexcept = default;

		void update(void) noexcept;
		auto create_object(object_share_ptr& parent) noexcept -> object_share_ptr;
		auto clone_object(library::string const& name, object_share_ptr& parent) noexcept -> object_share_ptr;
		//inline auto find_object(library::string const& name) noexcept -> library::vector<framework::object>& {
		//	auto result = _object.find(name);
		//	return result->_second;
		//}
		template <typename type>
		inline auto find_system(void) noexcept -> library::vector<framework::object*>& {
			auto result = _system.find(system::type_id<type>());
			return result->_second;
		}
	};
}