#pragma once
#include "library/pattern/singleton.h"
#include "library/json.hpp"
#include "library/container/read_copy_update.h"
#include "library/container/hash_table.h"
#include "library/container/pointer.h"
#include "library/container/string.h"
#include "library/system/guid.h"
#include "library/container/thread-local/pool.h"
#include <variant>

namespace framework {
	class base {
	public:
		explicit base(void) noexcept = default;
		explicit base(base const&) noexcept = default;
		explicit base(base&&) noexcept = default;
		auto operator=(base const&) noexcept -> base & = default;
		auto operator=(base&&) noexcept -> base & = default;
		virtual ~base(void) noexcept = default;

		virtual auto type(void) noexcept -> char const* const = 0;
	};

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

			virtual auto allocate(nlohmann::json const& json) noexcept -> base* = 0;
			virtual void deallocate(base* const pointer) noexcept = 0;
		};
		template <typename type>
		class pool : public pooli {
			library::pool<type, true, false> _pool;
			library::_thread_local::pool<type, 1024, true, false>& _pool2 = library::_thread_local::pool<type, 1024, true, false>::instance();
		public:
			pool(void) noexcept = default;
			pool(pool const&) noexcept = delete;
			pool(pool&&) noexcept = delete;
			auto operator=(pool const&) noexcept -> pool & = delete;
			auto operator=(pool&&) noexcept -> pool & = delete;
			virtual ~pool(void) noexcept override = default;

			template<typename... argument>
			auto allocate(argument&&... arg) noexcept -> type* {
				//return _pool.allocate(std::forward<argument>(arg)...);
				return _pool2.allocate(std::forward<argument>(arg)...);
			}
			virtual auto allocate(nlohmann::json const& json) noexcept ->  base* override {
				//return _pool.allocate(json);
				return _pool2.allocate(json);

			}
			virtual void deallocate(base* const pointer) noexcept override {
				//_pool.deallocate(static_cast<type*>(pointer));
				_pool2.deallocate(static_cast<type*>(pointer));
			}
		};

		library::unorder_map<library::string, library::unique_pointer<pooli>> _pool;

		explicit pools(void) noexcept = default;
		explicit pools(pools const&) noexcept = delete;
		explicit pools(pools&&) noexcept = delete;
		auto operator=(pools const&) noexcept -> pools & = delete;
		auto operator=(pools&&) noexcept -> pools & = delete;
		~pools(void) noexcept = default;
	public:
		template<typename type>
		void regist(void) noexcept {
			_pool.emplace(type::static_type(), library::make_unique<pool<type>>());
		}

		template<typename... argument>
		auto allocate(library::string_view key, nlohmann::json const& json) noexcept -> base* {
			auto result = _pool.find(key);
			auto pointer = result->_second->allocate(json);
			return pointer;
		};
		template<typename type, typename... argument>
		auto allocate(argument&&... arg) noexcept -> type* {
			auto result = _pool.find(type::static_type());
			auto pointer = static_cast<pool<type>&>(*result->_second).allocate(std::forward<argument>(arg)...);
			return pointer;
		};
		void deallocate(base* pointer) noexcept;
	};

	template<typename type>
	class pointer {
		template<typename other>
		friend class pointer;
		std::variant<library::guid, library::rcu_pointer<type>> _value;
	public:
		explicit pointer(library::guid guid) noexcept
			: _value(std::in_place_type<library::guid>, guid) {
		}
		template<typename other>
		explicit pointer(other* pointer) noexcept
			: _value(std::in_place_type<library::rcu_pointer<type>>, pointer) {
		}
		template<typename other>
		explicit pointer(library::rcu_pointer<other> rcu_pointer) noexcept
			: _value(std::in_place_type<library::rcu_pointer<type>>, rcu_pointer) {
		}
		template<typename other>
		inline pointer(pointer<other>& rhs) noexcept {
			if (std::holds_alternative<library::guid>(rhs._value)) 
				_value.emplace<library::guid>(std::get<library::guid>(rhs._value));
			else
				_value.emplace<library::rcu_pointer<type>>(std::get<library::rcu_pointer<other>>(rhs._value));
		};
		template<typename other>
		inline pointer(pointer<other>&& rhs) noexcept {
			if (std::holds_alternative<library::guid>(rhs._value))
				_value.emplace<library::guid>(std::move(std::get<library::guid>(rhs._value)));
			else
				_value.emplace<library::rcu_pointer<type>>(std::move(std::get<library::rcu_pointer<other>>(rhs._value)));
		};
		//template<typename other>
		//inline auto operator=(pointer<other> const& rhs) noexcept -> pointer& {
		//	_value = rhs._value;
		//	return *this;
		//}
		//template<typename other>
		//inline auto operator=(pointer<other>&& rhs) noexcept -> pointer& {
		//	_value = std::move(rhs._value);
		//	return *this;
		//};

		inline operator bool(void) noexcept {
			if (std::holds_alternative<library::guid>(_value))
				__debugbreak();
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			return static_cast<bool>(pointer);
		}
		inline bool operator==(nullptr_t) noexcept {
			if (std::holds_alternative<library::guid>(_value))
				__debugbreak();
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			return nullptr == pointer;
		}
		inline auto operator*(void) const noexcept -> type& {
			if (std::holds_alternative<library::guid>(_value)) {
				__debugbreak();
			}
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			return *pointer;
		}
		inline auto operator->(void) const noexcept -> type* const {
			if (std::holds_alternative<library::guid>(_value)) {
				__debugbreak();
			}
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			return pointer.operator->();
		}
		inline bool valid(void) const noexcept {
			if (std::holds_alternative<library::guid>(_value)) {
				__debugbreak();
			}
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			return pointer.valid();
		}

		inline void destory(void) noexcept {
			if (std::holds_alternative<library::guid>(_value)) {
				__debugbreak();
			}
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			pointer.invalid([](type* pointer) noexcept {
				pools::instance().deallocate(pointer);
				});
		}
		inline auto data(void) noexcept ->type* {
			if (std::holds_alternative<library::guid>(_value)) {
				__debugbreak();
			}
			auto& pointer = std::get<library::rcu_pointer<type>>(_value);
			return pointer.data();
		}
	};

	template<typename parent, typename child, library::string_literal name>
	class inherit : public parent {
		inline static bool _ = (pools::instance().template regist<child>(), true);
	public:
		using parent::parent;
		virtual auto type(void) noexcept -> char const* const override {
			return name._value;
		}
		static constexpr auto static_type(void) noexcept -> char const* const {
			return name._value;
		}
	};
}