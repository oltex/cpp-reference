#pragma once
#include "library/pattern/singleton.h"
#include "library/container/list.h"
#include "library/container/pool.h"
#include "library/container/pointer.h"
#include "library/container/string.h"
#include "library/container/intrusive/pointer.h"
#include "library/container/intrusive/pointer_list.h"
#include "library/container/hash_table.h"

namespace framework {
	class component : public library::intrusive::pointer_hook<0>, public library::intrusive::list_hook<0> {
	protected:
		using size_type = unsigned int;
	public:
		explicit component(void) noexcept = default;
		explicit component(component const&) noexcept = delete;
		explicit component(component&&) noexcept = delete;
		auto operator=(component const&) noexcept -> component & = delete;
		auto operator=(component&&) noexcept -> component & = delete;
		virtual ~component(void) noexcept = default;

		inline virtual auto name(void) noexcept -> char const* const = 0;

		template<size_t index>
		inline void destruct(void) noexcept {};
		template<>
		inline void destruct<0>(void) noexcept {
			library::destruct(*this);
		};
		template<size_t index>
		inline static void deallocate(component* pointer) noexcept {};
		template<>
		inline static void deallocate<0>(component* pointer) noexcept;
	};

	class components final : public library::singleton<components> {
		friend class library::singleton<components>;
		friend class object;
		friend class component;
		template<typename type, library::string_literal name, typename base>
		friend class componentr;
		class pools {
		protected:
			using size_type = unsigned int;
		public:
			inline explicit pools(void) noexcept = default;
			inline explicit pools(pools const&) noexcept = delete;
			inline explicit pools(pools&&) noexcept = delete;
			inline auto operator=(pools const&) noexcept -> pools & = delete;
			inline auto operator=(pools&&) noexcept -> pools & = delete;
			inline virtual ~pools(void) = default;

			//inline virtual auto allocate1(framework::component* value) noexcept -> framework::component* = 0;
			inline virtual void deallocate(framework::component* const value) noexcept = 0;
		};
		template <typename type>
		class pool : public pools, public library::pool<type, false> {
			using base = library::pool<type, false>;
			using size_type = pools::size_type;
		public:
			using base::base;
			inline virtual ~pool(void) = default;

			inline virtual void deallocate(framework::component* const value) noexcept override {
				base::deallocate(static_cast<type*>(value));
			}
		};

		using size_type = unsigned int;
		library::unorder_map<library::string, library::unique_pointer<pools>> _component;

		explicit components(void) noexcept = default;
		explicit components(components const&) noexcept = delete;
		explicit components(components&&) noexcept = delete;
		auto operator=(components const&) noexcept -> components & = delete;
		auto operator=(components&&) noexcept -> components & = delete;
		~components(void) noexcept = default;

		template<typename type>
		inline void register_component(void) noexcept {
			_component.emplace(type::static_name(), new pool<type>);
		}
		template<typename type, typename... argument>
		inline auto allocate_component(argument&&... arg) noexcept -> library::intrusive::share_pointer<component, 0> {
			auto result = _component.find(type::static_name());
			auto pointer = static_cast<pool<type>*>(&*result->_second)->allocate();
			library::construct<type>(*pointer, std::forward<argument>(arg)...);
			return library::intrusive::share_pointer<component, 0>(pointer);
		}
		void deallocate_component(framework::component* component) noexcept;
	};

	template<>
	inline static void component::deallocate<0>(component* pointer) noexcept {
		components::instance().deallocate_component(pointer);
	};

	template<typename type, library::string_literal type_name, typename base = component>
	class componentr : public base {
		struct regist {
			inline explicit regist(void) noexcept {
				components::instance().register_component<type>();
			}
		};
		inline static regist _regist{};
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