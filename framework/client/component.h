#pragma once
#include "library/pattern/singleton.h"
#include "library/container/list.h"
#include "library/container/pool.h"
#include "library/container/pointer.h"
#include "library/container/string.h"
#include "library/container/hash_table.h"
#include "library/container/read_copy_update.h"
#include "library/system/guid.h"

namespace framework {
	class component : public library::rcu_base<> {
	protected:
		library::guid _guid;
		using size_type = unsigned int;
	public:
		explicit component(void) noexcept;
		explicit component(component const&) noexcept = delete;
		explicit component(component&&) noexcept = delete;
		auto operator=(component const&) noexcept -> component & = delete;
		auto operator=(component&&) noexcept -> component & = delete;
		virtual ~component(void) noexcept = default;

		virtual auto type(void) noexcept -> char const* const = 0;
		auto guid(void) noexcept -> library::guid&;
		virtual void edit(void) noexcept {};
	};

	class components final : public library::singleton<components> {
		friend class library::singleton<components>;
		friend class object;
		friend class component;
		template<typename type, library::string_literal name, typename base>
		friend class componentr;
		class pools {
		public:
			explicit pools(void) noexcept = default;
			explicit pools(pools const&) noexcept = delete;
			explicit pools(pools&&) noexcept = delete;
			auto operator=(pools const&) noexcept -> pools & = delete;
			auto operator=(pools&&) noexcept -> pools & = delete;
			virtual ~pools(void) = default;

			virtual void deallocate(framework::component* const value) noexcept = 0;
		};
		template <typename type>
		class pool : public pools, public library::pool<type, true, false> {
			using base = library::pool<type, true, false>;
		public:
			using base::base;
			virtual ~pool(void) = default;

			template<typename... argument>
			auto allocate(argument&&... arg) noexcept -> type* {
				return base::allocate(std::forward<argument>(arg)...);
			}
			//virtual auto allocate(nlohmann::json& json) noexcept -> library::rcu_pointer<resource> override {
			//	auto pointer = _pool.allocate(json);
			//	return library::rcu_pointer<type>(pointer);
			//}
			virtual void deallocate(framework::component* const pointer) noexcept override {
				base::deallocate(static_cast<type*>(pointer));
			}
		};

		using size_type = unsigned int;
		library::unorder_map<library::string, library::unique_pointer<pools>> _pool;
		library::unorder_map<library::guid, library::rcu_pointer<component>> _guid;


		explicit components(void) noexcept = default;
		explicit components(components const&) noexcept = delete;
		explicit components(components&&) noexcept = delete;
		auto operator=(components const&) noexcept -> components & = delete;
		auto operator=(components&&) noexcept -> components & = delete;
		~components(void) noexcept = default;

		template<typename type>
		void regist(void) noexcept {
			_pool.emplace(type::static_type(), library::make_unique<pool<type>>());
		}
		template<typename type, typename... argument>
		auto create(argument&&... arg) noexcept -> library::rcu_pointer<type> {
			auto result = _pool.find(type::static_type());
			auto pointer = static_cast<pool<type>&>(*result->_second).allocate(std::forward<argument>(arg)...);
			library::rcu_pointer<type> rcu_pointer(pointer);
			_guid.emplace(pointer->guid(), pointer);
			return rcu_pointer;
		}
		void destory(library::rcu_pointer<component> pointer) noexcept;
	};

	template<typename _type, library::string_literal type_name, typename base = component>
	class componentr : public base {
		inline static bool _ = (components::instance().template regist<_type>(), true);
	public:
		using base::base;
		virtual auto type(void) noexcept -> char const* const override {
			return type_name._value;
		}
		static constexpr auto static_type(void) noexcept -> char const* {
			return type_name._value;
		}
	};
}