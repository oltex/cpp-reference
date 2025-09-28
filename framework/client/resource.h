#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"

namespace framework {
	class mesh;
	class resource {
	public:
		explicit resource(void) noexcept = default;
		explicit resource(resource const&) noexcept = delete;
		explicit resource(resource&&) noexcept = delete;
		auto operator=(resource const&) noexcept -> resource & = delete;
		auto operator=(resource&&) noexcept -> resource & = delete;
		virtual ~resource(void) noexcept = default;
	};

	class resources : public library::singleton<resources> {
		friend class library::singleton<resources>;
		library::unorder_map<library::string, library::unique_pointer<resource>> _resource;

		explicit resources(void) noexcept = default;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept = default;
	public:
		void load_sound(char const* const name, char const* const path) noexcept;
		template<typename type, typename... argument>
		inline void create_resource(char const* const name, argument&&... arg) noexcept {
			library::unique_pointer
			_resource.emplace(library::_piecewise_construct, library::forward_as_tuple(name),);
		};
	};
}