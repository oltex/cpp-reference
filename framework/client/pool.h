#pragma once
#include "library/pattern/singleton.h"
#include "library/json.hpp"

namespace framework {
	class pools : public library::singleton<pools> {
		friend class library::singleton<pools>;
		class pooli {
		public:
			pooli(void) noexcept = default;
			pooli(pooli const&) noexcept = delete;
			pooli(pooli&&) noexcept = delete;
			auto operator=(pooli const&) noexcept -> pooli & = delete;
			auto operator=(pooli&&) noexcept -> pooli & = delete;
			virtual ~pooli(void) noexcept = default;

			virtual auto allocate(nlohmann::json& json) noexcept -> framework::resource* = 0;
			virtual void deallocate(framework::resource* const pointer) noexcept = 0;
		};
		template <typename type>
		class pool : public pooli {
			library::pool<type, true, false> _pool;
		public:
			pool(void) noexcept = default;
			pool(pool const&) noexcept = delete;
			pool(pool&&) noexcept = delete;
			auto operator=(pool const&) noexcept -> pool & = delete;
			auto operator=(pool&&) noexcept -> pool & = delete;
			virtual ~pool(void) noexcept override = default;

			template<typename... argument>
			auto allocate(library::string_view name, argument&&... arg) noexcept -> type* {
				return _pool.allocate(name, std::forward<argument>(arg)...);
			}
			virtual auto allocate(nlohmann::json& json) noexcept ->  framework::resource* override {
				return _pool.allocate(json);
			}
			virtual void deallocate(framework::resource* const pointer) noexcept override {
				_pool.deallocate(static_cast<type*>(pointer));
			}
		};
	};
}