#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"
#include "library/system/guid.h"
#include "library/container/intrusive/pointer.h"

namespace framework {
	class resource : public library::intrusive::pointer_hook<0> {
		unsigned long long _generation;
		library::guid _guid;
	public:
		explicit resource(void) noexcept;
		explicit resource(resource const&) noexcept = delete;
		explicit resource(resource&&) noexcept = delete;
		auto operator=(resource const&) noexcept -> resource & = delete;
		auto operator=(resource&&) noexcept -> resource & = delete;
		virtual ~resource(void) noexcept = default;

		virtual auto type_name(void) noexcept -> char const* const = 0;
		auto guid(void) noexcept -> library::guid&;

		template<size_t index>
		inline void destruct(void) noexcept {};
		template<>
		inline void destruct<0>(void) noexcept {
			_generation++;
		};
		template<size_t index>
		inline static void deallocate(resource* pointer) noexcept {};
		template<>
		inline static void deallocate<0>(resource* pointer) noexcept {

		}
	};

	class resources : public library::singleton<resources> {
		friend class library::singleton<resources>;
		friend class asset;
		class segments {
		protected:
			using size_type = unsigned int;
		public:
			inline explicit segments(void) noexcept = default;
			inline explicit segments(segments const&) noexcept = delete;
			inline explicit segments(segments&&) noexcept = delete;
			inline auto operator=(segments const&) noexcept -> segments & = delete;
			inline auto operator=(segments&&) noexcept -> segments & = delete;
			inline virtual ~segments(void) = default;
		};
		template <typename type>
		class segment : public segments {
			using size_type = segments::size_type;
			library::pool<type, false> _pool;
			library::unorder_map<library::guid, library::intrusive::share_pointer<type, 0>> _guid;
			library::unorder_map<library::string, library::intrusive::share_pointer<type, 0>> _name;
		public:
			//using base::base;
			inline virtual ~segment(void) = default;

			template<typename... argument>
			inline auto allocate(char const* const name, argument&&... arg) noexcept -> type* {
				auto pointer = _pool.allocate(std::forward<argument>(arg)...);
				library::intrusive::share_pointer<type, 0> share_pointer(pointer);
				_guid.emplace(pointer->guid(), share_pointer);
				_name.emplace(name, share_pointer);
				return share_pointer.get();
			}
			inline auto deallocate(type* )

		};

		library::unorder_map<library::string, library::unique_pointer<segments>> _segment;
		library::unorder_map<library::guid, library::share_pointer<resource>> _resource;

		explicit resources(void) noexcept;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept = default;
	public:
		template<typename type>
		inline void regist_resource(void) noexcept {
			_segment.emplace(type::static_name(), library::make_unique<segment<type>>());
		}

		template<typename type, typename... argument>
		inline auto create_resource(argument&&... arg) noexcept -> library::share_pointer<type> {
			{
				auto result = _segment.find(type::static_name());
				/*auto pointer =*/ static_cast<segment<type>&>(*result->_second).allocate("test", std::forward<argument>(arg)...);
			}
			auto pointer = library::make_share<type>(std::forward<argument>(arg)...);
			_resource.emplace(pointer->guid(), pointer);
			return pointer;
		};
		//template<typename type>
		//auto find_resource(library::guid& guid) noexcept -> library::share_pointer<type> {
		//	auto result = _resource.find(guid);
		//	return result->_second;
		//}

	};

	template<typename type, library::string_literal name, typename base = resource>
	class resourcer : public base {
		struct regist {
			inline explicit regist(void) noexcept {
				resources::instance().regist_resource<type>();
			}
		};
		inline static regist _regist{};
	public:
		using base::base;
		virtual auto type_name(void) noexcept -> char const* const override {
			return name._value;
		}
		static constexpr auto static_name(void) noexcept -> char const* {
			return name._value;
		}
	};
}