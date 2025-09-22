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
		auto create_system(void) noexcept {

		}
		template <typename type>
		inline auto find_system(void) noexcept -> library::vector<framework::object*>& {
			auto result = _system.find(system::type_id<type>());
			return result->_second;
		}
	};
}

namespace implement {
	class scene_manager {
		scene* _current_scene;
		scene* _previous_scene;
	public:
		inline explicit scene_manager(void) noexcept = default;
		inline explicit scene_manager(scene_manager const&) noexcept = delete;
		inline explicit scene_manager(scene_manager&&) noexcept = delete;
		inline auto operator=(scene_manager const&) noexcept -> scene_manager & = delete;
		inline auto operator=(scene_manager&&) noexcept -> scene_manager & = delete;
		inline virtual ~scene_manager(void) noexcept = default;

		inline void update(void) noexcept {
			if (nullptr != _previous_scene)
				_current_scene = _previous_scene;
			if (nullptr != _current_scene)
				_current_scene->update();
		}

		inline void create_scene(library::string path) noexcept {
			auto _previous_scene = new framework::scene;
		}
		inline auto add_object(object_share_ptr& object) noexcept {
			//_current_scene->add_object();
		}
		inline auto add_system(void) noexcept {
			//_current_scene->add_object();
		}
	};
}