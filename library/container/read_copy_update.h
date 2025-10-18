#pragma once
#include "../container/lockfree/queue.h"
#include "../container/lockfree/free_list.h"
#include "../container/priority_queue.h"

//struct retire {
//	void* _pointer;
//	unsigned long long _epoch;
//	void (*deleter)(void*);
//};
namespace library {
	class read_copy_update {
		struct context {
			struct retire {
				void* _pointer;
				unsigned long long _epoch;
			};
			library::lockfree::free_list<unsigned long long, true, false>& _list;
			unsigned long long& _slot;
			library::priority_queue < retire, [](auto& lhs, auto& rhs) { return lhs._epoch < rhs._epoch; } > _retire;

			inline context(library::lockfree::free_list<unsigned long long, true, false>& list)
				: _list(list), _slot(*_list.allocate()) {
			}
			inline explicit context(context const&) noexcept = delete;
			inline explicit context(context&&) noexcept = delete;
			inline auto operator=(context const&) noexcept -> context & = delete;
			inline auto operator=(context&&) noexcept -> context & = delete;
			inline ~context(void) {
				_list.deallocate(&_slot);
				while (!_retire.empty()) {
					auto epoch = _retire.top()._epoch;

					unsigned long long minimum = ULLONG_MAX;
					for (auto& iter : _list)
						minimum = library::minimum(iter, minimum);

					if (epoch < minimum) {
						delete _retire.top()._pointer;
						_retire.pop();
					}
				}
			}

			//inline void reclaim(void) noexcept {
			//	static library::priority_queue<retire> retire_queue;
			//}
		};

		unsigned long long _epoch;
		library::lockfree::free_list<unsigned long long, true, false> _list;
	public:
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

		inline auto instance(void) noexcept -> context& {
			thread_local context epoch(_list);
			return epoch;
		}
	public:
		inline void lock(void) noexcept {
			auto& context = instance();
			context._slot = library::interlock_increment(_epoch);
		}
		inline void unlock(void) noexcept {
			auto& context = instance();
			context._slot = ULLONG_MAX;

			if (!context._retire.empty()) {
				unsigned long long minimum = ULLONG_MAX;
				for (auto& iter : _list)
					minimum = library::minimum(iter, minimum);

				while (!context._retire.empty()) {
					auto epoch = context._retire.top()._epoch;
					if (epoch < minimum) {
						delete context._retire.top()._pointer;
						context._retire.pop();
					}
					else
						break;
				}
			}
		}
		inline void retire(void* pointer) noexcept {
			auto& context = instance();
			context._retire.emplace(pointer, library::interlock_increment(_epoch));
		}
	};
}