#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"

namespace framework {
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
		friend class asset;
		library::unorder_map<library::string, library::share_pointer<resource>> _resource;

		explicit resources(void) noexcept;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept = default;
	public:
		template<typename type, typename... argument>
		inline auto create_resource(char const* const name, argument&&... arg) noexcept -> library::share_pointer<type> {
			auto pointer = library::make_share<type>(std::forward<argument>(arg)...);
			_resource.emplace(name, pointer);
			return pointer;
		};
		template<typename type>
		auto find_resource(char const* const name) noexcept -> library::share_pointer<type> {
			auto result = _resource.find(name);
			return result->_second;
		}
	};
}