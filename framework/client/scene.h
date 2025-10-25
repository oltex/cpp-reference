#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/vector.h"
#include "library/container/intrusive/list.h"
#include "library/container/intrusive/pointer_list.h"
#include "library/container/pointer.h"
#include "object.h"
#include "system.h"

#include "texture.h"

namespace framework {
	struct scene {
		using size_type = unsigned int;
		library::intrusive::pointer_list<object, 0, 0> _object;
		library::intrusive::pointer_list<system, 0, 0> _system;
		library::intrusive::pointer_list<system, 0, 0> _render_system;

		explicit scene(void) noexcept = default;
		explicit scene(scene const&) noexcept = delete;
		explicit scene(scene&&) noexcept = delete;
		auto operator=(scene const&) noexcept -> scene & = delete;
		auto operator=(scene&&) noexcept -> scene & = delete;
		~scene(void) noexcept = default;
	};

	class scenes : public library::singleton<scenes> {
		friend class client;
		friend class library::singleton<scenes>;
		library::unique_pointer<scene> _current_scene;
		library::unique_pointer<scene> _next_scene;

		explicit scenes(void) noexcept;
		explicit scenes(scenes const&) noexcept = delete;
		explicit scenes(scenes&&) noexcept = delete;
		auto operator=(scenes const&) noexcept -> scenes & = delete;
		auto operator=(scenes&&) noexcept -> scenes & = delete;
		~scenes(void) noexcept = default;
	public:
		void update(void) noexcept;
		void render_system(void) noexcept;

		void create_scene(library::string const& path) noexcept;
		template<typename type>
		inline void create_update_system(void) noexcept {
			library::intrusive::share_pointer<system, 0> pointer(new type);
			_current_scene->_system.push_back(pointer);
		}
		template<typename type>
		inline auto create_render_system(void) noexcept -> library::intrusive::share_pointer<type, 0> {
			library::intrusive::share_pointer<system, 0> pointer(new type);
			_current_scene->_render_system.push_back(pointer);
			return pointer;
		}
		auto create_object(void) noexcept -> library::intrusive::share_pointer<object, 0>;
		auto clone_object(library::intrusive::share_pointer<object, 0>& origin) noexcept -> library::intrusive::share_pointer<object, 0>;
		void destory_object(library::intrusive::share_pointer<object, 0>& pointer) noexcept;

		texture _screen;
		texture _depth;
	};
}