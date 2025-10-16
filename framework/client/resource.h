#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"
#include "library/system/guid.h"

namespace framework {
	class resource {
		library::guid _guid;
	public:
		explicit resource(void) noexcept;
		explicit resource(resource const&) noexcept = delete;
		explicit resource(resource&&) noexcept = delete;
		auto operator=(resource const&) noexcept -> resource & = delete;
		auto operator=(resource&&) noexcept -> resource & = delete;
		virtual ~resource(void) noexcept = default;

		auto guid(void) noexcept -> library::guid&;
	};

	class resources : public library::singleton<resources> {
		friend class library::singleton<resources>;
		friend class asset;
		library::unorder_map<library::guid, library::share_pointer<resource>> _resource;

		explicit resources(void) noexcept;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept = default;
	public:
		template<typename type, typename... argument>
		inline auto create_resource(argument&&... arg) noexcept -> library::share_pointer<type> {
			auto pointer = library::make_share<type>(std::forward<argument>(arg)...);
			_resource.emplace(pointer->guid(), pointer);
			return pointer;
		};
		template<typename type>
		auto find_resource(library::guid& guid) noexcept -> library::share_pointer<type> {
			auto result = _resource.find(guid);
			return result->_second;
		}
	};

	template<typename type, library::string_literal type_name, typename base = resource>
	class resourcer : public base {
		//struct regist {
		//	inline explicit regist(void) noexcept {
		//		resources::instance().register_component<type>();
		//	}
		//};
		//inline static regist _regist{};
	public:
		using base::base;
		inline virtual auto name(void) noexcept -> char const* const override {
			return type_name._value;
		}
		inline static constexpr auto static_name(void) noexcept -> char const* {
			return type_name._value;
		}
	};
}