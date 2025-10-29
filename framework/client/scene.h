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

#include "level.h"

namespace framework {
	struct scene {
		using size_type = unsigned int;
		explicit scene(void) noexcept = default;
		explicit scene(scene const&) noexcept = delete;
		explicit scene(scene&&) noexcept = delete;
		auto operator=(scene const&) noexcept -> scene & = delete;
		auto operator=(scene&&) noexcept -> scene & = delete;
		~scene(void) noexcept;
	};

	class scenes : public library::singleton<scenes> {
		friend class client;
		friend class library::singleton<scenes>;

		library::vector<library::rcu_pointer<object>> _object;
		library::intrusive::pointer_list<system, 0, 0> _system;
		library::intrusive::pointer_list<system, 0, 0> _render_system;

		explicit scenes(void) noexcept;
		explicit scenes(scenes const&) noexcept = delete;
		explicit scenes(scenes&&) noexcept = delete;
		auto operator=(scenes const&) noexcept -> scenes & = delete;
		auto operator=(scenes&&) noexcept -> scenes & = delete;
		~scenes(void) noexcept;
	public:
		void update(void) noexcept;
		void render(void) noexcept;
		void edit(void) noexcept;

		void open(framework::level& level) noexcept;
		template<typename type>
		inline void create_update_system(void) noexcept {
			library::intrusive::share_pointer<system, 0> pointer(new type);
			_system.push_back(pointer);
		}
		template<typename type>
		inline auto create_render_system(void) noexcept -> library::intrusive::share_pointer<type, 0> {
			library::intrusive::share_pointer<system, 0> pointer(new type);
			_render_system.push_back(pointer);
			return pointer;
		}
		auto create_object(void) noexcept -> library::rcu_pointer<object>;
		//auto clone_object(library::intrusive::share_pointer<object, 0>& origin) noexcept -> library::intrusive::share_pointer<object, 0>;
		void destory_object(library::rcu_pointer<object> pointer) noexcept;
	};
}