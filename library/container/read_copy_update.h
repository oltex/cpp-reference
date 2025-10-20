#pragma once
#include "../function.h"
#include "../pattern/singleton.h"
#include "../pattern/thread-local/singleton.h"
#include "../container/lockfree/queue.h"
#include "../container/lockfree/free_list.h"
#include "../container/priority_queue.h"
#include <functional>

namespace library {
	inline auto default_deleter(void* pointer) noexcept {
		delete pointer;
	}

//	class rcu_base {
//		template<typename type>
//		friend class rcu_pointer;
//		unsigned long long _generation;
//	public:
//#pragma warning(suppress: 4624)
//		inline rcu_base(void) noexcept {
//		};
//		inline rcu_base(rcu_base const&) noexcept = default;
//		inline rcu_base(rcu_base&&) noexcept = default;
//		inline auto operator=(rcu_base const&) noexcept -> rcu_base & = default;
//		inline auto operator=(rcu_base&&) noexcept -> rcu_base & = default;
//		inline ~rcu_base(void) noexcept = default;
//	private:
//		inline auto generation(void) const noexcept -> unsigned long long {
//			return _generation;
//		}
//		inline bool valid(unsigned long long generation) const noexcept {
//			return _generation == generation;
//		}
//		inline bool invalid(unsigned long long generation) noexcept {
//			return generation == library::interlock_compare_exhange(_generation, generation + 1, generation);
//		}
//	};
//
//	template<typename type>
//	class rcu_pointer {
//		template <typename other>
//		friend class rcu_pointer;
//		type* _pointer;
//		unsigned long long _generation;
//	public:
//		inline rcu_pointer(type* pointer) noexcept
//			: _pointer(pointer), _generation(pointer->generation()) {
//		};
//		inline rcu_pointer(rcu_pointer const&) noexcept = default;
//		inline rcu_pointer(rcu_pointer&&) noexcept = default;
//		inline auto operator=(rcu_pointer const&) noexcept -> rcu_pointer & = default;
//		inline auto operator=(rcu_pointer&&) noexcept -> rcu_pointer & = default;
//		inline ~rcu_pointer(void) noexcept = default;
//		template<typename other>
//		inline rcu_pointer(rcu_pointer<other> const& rhs) noexcept
//			: _pointer(static_cast<type*>(rhs._pointer)), _generation(rhs._generation) {
//		};
//		template<typename other>
//		inline rcu_pointer(rcu_pointer<other>&& rhs) noexcept
//			: _pointer(static_cast<type*>(library::exchange(rhs._pointer, nullptr))), _generation(rhs._generation) {
//		};
//		template<typename other>
//		inline auto operator=(rcu_pointer<other> const& rhs) noexcept -> rcu_pointer& {
//			_pointer = static_cast<type*>(rhs._pointer);
//			_generation = rhs._generation;
//			return *this;
//		}
//		template<typename other>
//		inline auto operator=(rcu_pointer<other>&& rhs) noexcept -> rcu_pointer& {
//			_pointer = static_cast<type*>(library::exchange(rhs._pointer, nullptr));
//			_generation = rhs._generation;
//			return *this;
//		};
//
//		inline operator bool(void) noexcept {
//			return _pointer->valid(_generation);
//		}
//		inline auto operator*(void) const noexcept -> type& {
//			return *_pointer;
//		}
//		inline auto operator->(void) const noexcept -> type* const {
//			return _pointer;
//		}
//		inline bool valid(void) const noexcept {
//			return _pointer->valid(_generation);
//		}
//		template<typename function>
//		inline void invalid(function && func) noexcept;
//	};

	class read_copy_update : public singleton<read_copy_update> {
		friend class singleton<read_copy_update>;
		struct context : public _thread_local::singleton<context> {
			friend class _thread_local::singleton<context>;
			struct retire{
				void* _pointer;
				unsigned long long _epoch;
				void (*_deleter)(void*);
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
						top._deleter(top._pointer);
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
						top._deleter(top._pointer);
						context._retire.pop();
					}
				}
			}
		}
		inline void retire(void* pointer, void (*deleter)(void*) = default_deleter) noexcept {
			auto& context = context::instance();
			context._retire.emplace(pointer, library::interlock_increment(_epoch), deleter);
		};
	};

	//template <typename type>
	//template <typename function>
	//inline void rcu_pointer<type>::invalid(function&& func) noexcept {
	//	if (_pointer->invalid(_generation))
	//		read_copy_update::instance().retire(_pointer, func);
	//}
}