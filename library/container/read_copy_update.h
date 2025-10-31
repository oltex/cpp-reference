#pragma once
#include "../function.h"
#include "../pattern/singleton.h"
#include "../pattern/thread-local/singleton.h"
#include "../container/lockfree/queue.h"
#include "../container/lockfree/free_list.h"
#include "../container/priority_queue.h"
#include <functional>
#include <iostream>

namespace library {
	inline auto default_deleter(void* pointer) noexcept {
		delete pointer;
	}

	template<typename type, auto deleter = nullptr>
	class rcu_pointer {
	public:
		template <typename other, auto>
		friend class rcu_pointer;
		type* _pointer;
		unsigned long long _generation;
		inline rcu_pointer(void) noexcept
			: _pointer(nullptr) {
		};
		template<typename other>
		inline rcu_pointer(other* pointer) noexcept
			: _pointer(static_cast<type*>(pointer)), _generation(_pointer->generation()) {
			if constexpr (type::self) {
				_pointer->pointer() = *this;
			}
		};
		//inline rcu_pointer(rcu_pointer const&) noexcept = default;
		//inline rcu_pointer(rcu_pointer&&) noexcept = default;
		//inline auto operator=(rcu_pointer const&) noexcept -> rcu_pointer & = default;
		//inline auto operator=(rcu_pointer&&) noexcept -> rcu_pointer & = default;
		inline ~rcu_pointer(void) noexcept {
			if constexpr (nullptr != deleter)
				invalid(deleter);
		};
		template<typename other, auto other_deleter>
		inline rcu_pointer(rcu_pointer<other, other_deleter> & rhs) noexcept
			: _pointer(static_cast<type*>(rhs._pointer)), _generation(rhs._generation) {
		};
		template<typename other, auto other_deleter>
		inline rcu_pointer(rcu_pointer<other, other_deleter>&& rhs) noexcept
			: _pointer(static_cast<type*>(library::exchange(rhs._pointer, nullptr))), _generation(rhs._generation) {
		};
		template<typename other, auto other_deleter>
		inline auto operator=(rcu_pointer<other, other_deleter> const& rhs) noexcept -> rcu_pointer& {
			_pointer = static_cast<type*>(rhs._pointer);
			_generation = rhs._generation;
			return *this;
		}
		template<typename other, auto other_deleter>
		inline auto operator=(rcu_pointer<other, other_deleter>&& rhs) noexcept -> rcu_pointer& {
			_pointer = static_cast<type*>(library::exchange(rhs._pointer, nullptr));
			_generation = rhs._generation;
			return *this;
		};

		inline operator bool(void) noexcept {
			return nullptr != _pointer && _pointer->valid(_generation);
		}
		inline bool operator==(nullptr_t) noexcept {
			return nullptr == _pointer;
		}
		inline auto operator*(void) const noexcept -> type& {
			return *_pointer;
		}
		inline auto operator->(void) const noexcept -> type* const {
			return _pointer;
		}
		inline bool valid(void) const noexcept {
			return _pointer->valid(_generation);
		}
		inline auto invalid(void) noexcept -> type* {
			if (_pointer->invalid(_generation))
				return _pointer;
			return nullptr;
		};
		template<typename function>
			requires std::invocable<function&, type*>
		inline void invalid(function&& func) noexcept;
		inline auto data(void) noexcept ->type* {
			return _pointer;
		}
	};
	/* generation must not be modified or destroyed */
	template<typename type = void>
	class rcu_base {
		template<typename type, auto>
		friend class rcu_pointer;
		unsigned long long _generation;
		library::rcu_pointer<type> _pointer;
		inline static constexpr bool self = true;
	public:
#pragma warning(suppress: 26495)
		inline rcu_base(void) noexcept {
		};
		inline rcu_base(rcu_base const&) noexcept = default;
		inline rcu_base(rcu_base&&) noexcept = default;
		inline auto operator=(rcu_base const&) noexcept -> rcu_base & = default;
		inline auto operator=(rcu_base&&) noexcept -> rcu_base & = default;
		inline ~rcu_base(void) noexcept = default;

		inline auto pointer(void) noexcept -> library::rcu_pointer<type>& {
			return _pointer;
		}
	private:
		inline auto generation(void) const noexcept -> unsigned long long {
			return _generation;
		}
		inline bool valid(unsigned long long generation) const noexcept {
			return _generation == generation;
		}
		inline bool invalid(unsigned long long generation) noexcept {
			return generation == library::interlock_compare_exhange(_generation, generation + 1, generation);
		}
	};
	template<>
	class rcu_base<void> {
		template<typename type, auto>
		friend class rcu_pointer;
		unsigned long long _generation;
		inline static constexpr bool self = false;
	public:
#pragma warning(suppress: 26495)
		inline rcu_base(void) noexcept {
		};
		inline rcu_base(rcu_base const&) noexcept = default;
		inline rcu_base(rcu_base&&) noexcept = default;
		inline auto operator=(rcu_base const&) noexcept -> rcu_base & = default;
		inline auto operator=(rcu_base&&) noexcept -> rcu_base & = default;
		inline ~rcu_base(void) noexcept = default;
	private:
		inline auto generation(void) const noexcept -> unsigned long long {
			return _generation;
		}
		inline bool valid(unsigned long long generation) const noexcept {
			return _generation == generation;
		}
		inline bool invalid(unsigned long long generation) noexcept {
			return generation == library::interlock_compare_exhange(_generation, generation + 1, generation);
		}
	};

	//class rcu_pool {
	//	struct node : public rcu_base {
	//	};
	//};

	class read_copy_update : public singleton<read_copy_update> {
		friend class singleton<read_copy_update>;
		struct context : public _thread_local::singleton<context> {
			friend class _thread_local::singleton<context>;
			struct retire {
				unsigned long long _epoch;
				std::function<void(void)> _deleter;
			};
			unsigned long long& _slot;
			library::priority_queue < retire, [](auto& lhs, auto& rhs) noexcept { return lhs._epoch < rhs._epoch; } > _retire;

			inline context(void) noexcept
				: _slot(*read_copy_update::instance()._list.allocate()) {
			}
			inline explicit context(context const&) noexcept = delete;
			inline explicit context(context&&) noexcept = delete;
			inline auto operator=(context const&) noexcept -> context & = delete;
			inline auto operator=(context&&) noexcept -> context & = delete;
			inline ~context(void) {
				auto& list = read_copy_update::instance()._list;
				list.deallocate(&_slot);
				while (!_retire.empty()) {
					unsigned long long minimum = ULLONG_MAX;
					for (auto& iter : list)
						minimum = library::minimum(iter, minimum);

					if (auto& top = _retire.top(); top._epoch < minimum) {
						top._deleter();
						_retire.pop();
					}
				}
			}
		};

		unsigned long long _epoch;
		library::lockfree::free_list<unsigned long long, true, false> _list; // TODO: replace with a lock-free list later.

		inline explicit read_copy_update(void) noexcept
			: _epoch(0), _list(16) {
			for (auto& iter : _list)
				iter = ULLONG_MAX;
		};
		inline explicit read_copy_update(read_copy_update const&) noexcept = delete;
		inline explicit read_copy_update(read_copy_update&&) noexcept = delete;
		inline auto operator=(read_copy_update const&) noexcept -> read_copy_update & = delete;
		inline auto operator=(read_copy_update&&) noexcept -> read_copy_update & = delete;
		inline ~read_copy_update(void) noexcept = default;
	public:
		inline void lock(void) noexcept {
			auto& context = context::instance();
			context._slot = library::interlock_increment(_epoch);
		}
		inline void unlock(void) noexcept {
			auto& context = context::instance();
			context._slot = ULLONG_MAX;

			//reclaim
			if (!context._retire.empty()) {
				unsigned long long minimum = ULLONG_MAX;
				for (auto& iter : _list)
					minimum = library::minimum(iter, minimum);

				while (!context._retire.empty()) {
					if (auto& top = context._retire.top(); top._epoch >= minimum)
						break;
					else {
						top._deleter();
						context._retire.pop();
					}
				}
			}
		}
		template<typename type, typename function>
		inline void retire(type* pointer, function&& func) noexcept {
			auto& context = context::instance();
			context._retire.emplace(library::interlock_increment(_epoch), std::bind(std::forward<function>(func), pointer));
		};
	};

	template <typename type, auto deleter>
	template<typename function>
		requires std::invocable<function&, type*>
	inline void rcu_pointer<type, deleter>::invalid(function&& func) noexcept {
		if (_pointer->invalid(_generation))
			read_copy_update::instance().retire(_pointer, func);
	}
}