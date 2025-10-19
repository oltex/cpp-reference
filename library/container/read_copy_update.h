#pragma once
#include "../pattern/singleton.h"
#include "../pattern/thread-local/singleton.h"
#include "../container/lockfree/queue.h"
#include "../container/lockfree/free_list.h"
#include "../container/priority_queue.h"

namespace library {
	inline auto default_deleter(void* pointer) noexcept {
		delete pointer;
	}
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
			: _epoch(0), _list(10) {
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
		}
	};
}