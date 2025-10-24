#pragma once
#include "library/pattern/singleton.h"
#include "library/container/hash_table.h"
#include "library/container/string.h"
#include "library/container/pointer.h"
#include "library/system/guid.h"
#include "library/container/read_copy_update.h"
#include "library/json.hpp"

namespace framework {
	class resource : public library::rcu_base {
	protected:
		library::guid _guid;
		library::string _name;
	public:
		resource(void) noexcept = default;
		resource(library::string_view name) noexcept;
		resource(nlohmann::json const& json) noexcept;
		resource(resource const&) noexcept = delete;
		resource(resource&&) noexcept = delete;
		auto operator=(resource const&) noexcept -> resource & = delete;
		auto operator=(resource&&) noexcept -> resource & = delete;
		virtual ~resource(void) noexcept = default;

		virtual auto type(void) noexcept -> char const* const = 0;
		auto name(void) noexcept -> library::string&;
		auto guid(void) noexcept -> library::guid&;
		virtual void save(nlohmann::json& json) noexcept;
	};

	class resources : public library::singleton<resources> {
		friend class library::singleton<resources>;
		class pools {
		public:
			pools(void) noexcept = default;
			pools(pools const&) noexcept = delete;
			pools(pools&&) noexcept = delete;
			auto operator=(pools const&) noexcept -> pools & = delete;
			auto operator=(pools&&) noexcept -> pools & = delete;
			virtual ~pools(void) noexcept = default;

			virtual auto allocate(nlohmann::json& json) noexcept -> library::rcu_pointer<resource> = 0;
			virtual void deallocate(framework::resource* const pointer) noexcept = 0;
		};
		template <typename type>
		class pool : public pools {
			library::pool<type, true, false> _pool;
		public:
			pool(void) noexcept = default;
			pool(pool const&) noexcept = delete;
			pool(pool&&) noexcept = delete;
			auto operator=(pool const&) noexcept -> pool & = delete;
			auto operator=(pool&&) noexcept -> pool & = delete;
			virtual ~pool(void) noexcept override = default;

			template<typename... argument>
			auto allocate(library::string_view name, argument&&... arg) noexcept -> library::rcu_pointer<type> {
				auto pointer = _pool.allocate(name, std::forward<argument>(arg)...);
				return library::rcu_pointer<type>(pointer);
			}
			virtual auto allocate(nlohmann::json& json) noexcept -> library::rcu_pointer<resource> override {
				auto pointer = _pool.allocate(json);
				return library::rcu_pointer<type>(pointer);
			}
			virtual void deallocate(framework::resource* const pointer) noexcept override {
				_pool.deallocate(static_cast<type*>(pointer));
			}
		};

		library::unorder_map<library::string, library::unique_pointer<pools>> _pool;
		library::unorder_map<library::guid, library::rcu_pointer<resource>> _guid;
		library::unorder_map<library::string, library::rcu_pointer<resource>> _name;

		enum column {
			name,
			type
		};
		std::vector<library::rcu_pointer<resource>> _item;

		resources(void) noexcept;
		resources(resources const&) noexcept = delete;
		resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept;
	public:
		template<typename type>
		void regist(void) noexcept {
			_pool.emplace(type::static_type(), library::make_unique<pool<type>>());
		}
		template<typename type, typename... argument>
		auto create(library::string_view name, argument&&... arg) noexcept -> library::rcu_pointer<type> {
			auto result = _pool.find(type::static_type());
			auto pointer = static_cast<pool<type>&>(*result->_second).allocate(name, std::forward<argument>(arg)...);
			return pointer;
		};
		void destory(library::rcu_pointer<resource> pointer) noexcept;
		template<typename type>
		auto find(library::guid guid) noexcept -> library::rcu_pointer<type> {
			auto result = _guid.find(guid);
			return result->_second;
		}
		template<typename type>
		auto find(library::string_view name) noexcept -> library::rcu_pointer<type> {
			auto result = _name.find(name);
			return result->_second;
		}
		void save(void) noexcept;
		void load(void) noexcept;

		void update(void) noexcept;
		void import_file(void) noexcept;
		void search(void) noexcept;
	};

	template<typename _type, library::string_literal name, typename base = resource>
	class resourcer : public base {
		inline static bool _ = (resources::instance().template regist<_type>(), true);
	public:
		using base::base;
		virtual auto type(void) noexcept -> char const* const override {
			return name._value;
		}
		static constexpr auto static_type(void) noexcept -> char const* const {
			return name._value;
		}
	};
}