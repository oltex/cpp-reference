#pragma once
#include "library/pattern/singleton.h"
#include "library/container/list.h"
#include "library/container/pool.h"
#include "library/container/intrusive/pointer.h"
#include "library/container/hash_table.h"

namespace framework {
	class component : public library::intrusive::pointer_hook<0> {
		using size_type = unsigned int;
		inline static size_type _static_type_id = 0;
	protected:
		size_type _type_id;
	public:
		explicit component(size_type const type_id) noexcept;
		explicit component(component const&) noexcept = delete;
		explicit component(component&&) noexcept = delete;
		auto operator=(component const&) noexcept -> component & = delete;
		auto operator=(component&&) noexcept -> component & = delete;
		virtual ~component(void) noexcept = default;

		template<typename type>
		inline static auto type_id(void) noexcept -> size_type {
			static size_type type_id = _static_type_id++;
			return type_id;
		}
		inline auto type_id(void) const noexcept -> size_type;
	};

	class components final : public library::singleton<components> {
		friend class library::singleton<components>;
		friend class object;
		class pool_interface {
		protected:
			using size_type = unsigned int;
		public:
			inline virtual ~pool_interface(void) = default;

			//inline virtual auto allocate1(framework::component* value) noexcept -> framework::component* = 0;
			//inline virtual void deallocate(framework::component* const value) noexcept = 0;
		};
		template <typename type>
		class pool : public pool_interface, public library::pool<type> {
			using base = library::pool<type>;
			using size_type = pool_interface::size_type;
		public:
			using base::base;
			inline virtual ~pool(void) = default;

			//inline virtual auto allocate1(framework::component* value) noexcept -> framework::component* override {
			//	return base::allocate(*value);
			//}
			//inline virtual void deallocate(framework::component* const value) noexcept override {
			//	base::deallocate(value);
			//}
		};

		using size_type = unsigned int;
		library::unorder_map<size_type, pool_interface*> _component;

		inline explicit components(void) noexcept = default;
		inline explicit components(components const&) noexcept = delete;
		inline explicit components(components&&) noexcept = delete;
		inline auto operator=(components const&) noexcept -> components & = delete;
		inline auto operator=(components&&) noexcept -> components & = delete;
		inline ~components(void) noexcept = default;

		template<typename type, typename... argument>
		inline auto allocate_component(argument&&... arg) noexcept -> library::intrusive::share_pointer<component, 0> {
			auto result = _component.find(framework::component::type_id<type>());
			if (_component.end() == result)
				result = _component.emplace(framework::component::type_id<type>(), new pool<type>);
			auto pointer = static_cast<pool<type>*>(result->_second)->allocate(std::forward<argument>(arg)...);
			return library::intrusive::share_pointer<component, 0>(pointer);
		}
		//inline auto deallocate_component(framework::component* component) noexcept {
		//	auto result = _component.find(component->type_id());
		//	if (_component.end() == result)
		//		__debugbreak();
		//	result->_second->deallocate(component);
		//}
	};
}