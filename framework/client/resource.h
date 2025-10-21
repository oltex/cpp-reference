#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"
#include "library/system/guid.h"
#include "library/container/intrusive/pointer.h"
#include "library/container/read_copy_update.h"

namespace framework {
	class resource : public library::rcu_base {
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
	};

	class resources : public library::singleton<resources> {
		friend class library::singleton<resources>;
		friend class asset;
		class pools {
		public:
			explicit pools(void) noexcept = default;
			explicit pools(pools const&) noexcept = delete;
			explicit pools(pools&&) noexcept = delete;
			auto operator=(pools const&) noexcept -> pools & = delete;
			auto operator=(pools&&) noexcept -> pools & = delete;
			virtual ~pools(void) = default;

			virtual void deallocate(framework::resource* const pointer) noexcept = 0;
		};
		template <typename type>
		class pool : public pools, public library::pool<type, true, false> {
		public:
			explicit pool(void) noexcept = default;
			explicit pool(pool const&) noexcept = delete;
			explicit pool(pool&&) noexcept = delete;
			auto operator=(pool const&) noexcept -> pool & = delete;
			auto operator=(pool&&) noexcept -> pool & = delete;
			virtual ~pool(void) = default;

			template<typename... argument>
			auto allocate(argument&&... arg) noexcept -> library::rcu_pointer<type> {
				auto pointer = library::pool<type, true, false>::allocate(std::forward<argument>(arg)...);
				return library::rcu_pointer<type>(pointer);
			}
			virtual void deallocate(framework::resource* const pointer) noexcept override {
				library::pool<type, true, false>::deallocate(static_cast<type*>(pointer));
			}
		};

		library::unorder_map<library::string, library::unique_pointer<pools>> _pool;
		library::unorder_map<library::guid, library::rcu_pointer<resource>> _guid;
		library::unorder_map<library::string, library::rcu_pointer<resource>> _name;

		explicit resources(void) noexcept;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept = default;
	public:
		template<typename type>
		void regist_resource(void) noexcept {
			_pool.emplace(type::static_name(), library::make_unique<pool<type>>());
		}
		template<typename type, typename... argument>
		auto create_resource(char const* const name, argument&&... arg) noexcept -> library::rcu_pointer<type> {
			auto result = _pool.find(type::static_name());
			auto pointer = static_cast<pool<type>&>(*result->_second).allocate(std::forward<argument>(arg)...);
			return pointer;
		};
		void destory_resource(library::rcu_pointer<resource> pointer) noexcept;
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