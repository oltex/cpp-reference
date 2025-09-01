#pragma once
#include "library/pattern/singleton.h"
#include "library/system/inputoutput_complet_port.h"
#include "library/system/thread.h"
#include "library/container/array.h"

#include "library/system/wait_on_address.h"
#include "library/container/thread-local/pool.h"
#include "library/container/lockfree/queue.h"
#include "library/container/priority_queue.h"
#include "library/system/time.h"
#include "library/system/interlock.h"
#include <functional>

class io_complet_port : public library::singleton<io_complet_port, true> {
	friend class library::singleton<io_complet_port, true>;
	using size_type = unsigned int;
	enum class type : unsigned char {
		close = 0, worker, scheduler
	};
	struct task {
		unsigned long _time;
		std::function<int(void)> _function;

		template <typename function, typename... argument>
		inline explicit task(function&& func, argument&&... arg) noexcept
			: _time(library::time_get_time()), _function(std::bind(std::forward<function>(func), std::forward<argument>(arg)...)) {
		};
		inline explicit task(task const&) noexcept = delete;
		inline explicit task(task&&) noexcept = delete;
		inline auto operator=(task const&) noexcept -> task & = delete;
		inline auto operator=(task&&) noexcept -> task & = delete;
		inline ~task(void) noexcept = default;

		inline bool execute(void) noexcept {
			for (;;) {
				switch (int time = _function()) {
				case 0:
					break;
				case -1:
					return false;
				default:
					_time += time;
					return true;
				}
			}
		}
	};
	class queue final : public library::lockfree::queue<task*, false> {
		using base = library::lockfree::queue<task*, false>;
		unsigned long _size;
	public:
		inline explicit queue(void) noexcept
			: _size(0), base() {
		};
		inline explicit queue(queue const&) noexcept = delete;
		inline explicit queue(queue&&) noexcept = delete;
		inline auto operator=(queue const&) noexcept -> queue & = delete;
		inline auto operator=(queue&&) noexcept -> queue & = delete;
		inline ~queue(void) noexcept = default;

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			library::interlock_increment(_size);
			base::emplace(std::forward<argument>(arg)...);
			library::wake_by_address_single(_size);
		}
		inline auto pop(void) noexcept -> task* {
			auto result = base::pop();
			library::interlock_decrement(_size);
			return result;
		}
		inline bool wait(unsigned long mili_second) noexcept {
			unsigned long compare = 0;
			library::wait_on_address(_size, compare, mili_second);
			return !(_size & 0x80000000);
		}
		inline void stop(void) noexcept {
			library::interlock_or(_size, 0x80000000);
			library::wake_by_address_all(_size);
		}
	};
public:
	library::inputoutput_complet_port _complet_port;
	library::array<library::thread, 0> _worker_thread;
	library::thread _scheduler_thread;
	queue _scheduler_queue;
public:
	class io_complet_object {
	protected:
		io_complet_port& _iocp;
	public:
		inline explicit io_complet_object(void) noexcept
			: _iocp(io_complet_port::instance()) {
		}
		inline explicit io_complet_object(io_complet_object const&) noexcept = delete;
		inline explicit io_complet_object(io_complet_object&&) noexcept = delete;
		inline auto operator=(io_complet_object const&) noexcept -> io_complet_object & = delete;
		inline auto operator=(io_complet_object&&) noexcept -> io_complet_object & = delete;
		inline ~io_complet_object(void) noexcept = default;

		inline virtual void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept = 0;
	};

	inline explicit io_complet_port(size_type concurrent, size_type worker) noexcept
		: _scheduler_thread(&io_complet_port::scheduler, 0, this), _complet_port(concurrent), _worker_thread(worker, &io_complet_port::worker, 0, this) {
	}
	inline explicit io_complet_port(io_complet_port const&) noexcept = delete;
	inline explicit io_complet_port(io_complet_port&&) noexcept = delete;
	inline auto operator=(io_complet_port const&) noexcept -> io_complet_port & = delete;
	inline auto operator=(io_complet_port&&) noexcept -> io_complet_port & = delete;
	inline ~io_complet_port(void) noexcept {
		HANDLE handle[128];
		for (size_type index = 0; index < _worker_thread.capacity(); ++index) {
			_complet_port.post_queue_state(0, 0, nullptr);
			handle[index] = _worker_thread[index].data();
		}
		library::handle::wait_for_multiple(_worker_thread.capacity(), handle, true, INFINITE);
	};

	inline void connect(io_complet_object& object, library::socket& socket, uintptr_t key) noexcept {
		_complet_port.connect(socket, (static_cast<unsigned long long>(type::worker) << 56) | (key << 47) | reinterpret_cast<uintptr_t>(&object));
	}
	inline void post(io_complet_object& object, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept {
		_complet_port.post_queue_state(transferred, (key << 47) | reinterpret_cast<uintptr_t>(&object), overlapped);
	}
	template <typename function, typename... argument>
	inline void execute(function&& func, argument&&... arg) noexcept {
		auto task = library::_thread_local::pool<io_complet_port::task>::instance().allocate(std::forward<function>(func), std::forward<argument>(arg)...);
		_complet_port.post_queue_state(0, 0xFF00000000000000ULL & (static_cast<unsigned long long>(type::scheduler) << 56), reinterpret_cast<OVERLAPPED*>(task));
	}
private:
	inline void worker(void) noexcept {
		for (;;) {
			auto [result, transferred, key, overlapped] = _complet_port.get_queue_state(INFINITE);
			switch (static_cast<type>(key >> 56)) {
			case type::close:
				return;
			case type::worker:
				reinterpret_cast<io_complet_object*>(0x00007FFFFFFFFFFFULL & key)->worker(result, transferred, (0x00FF800000000000ULL & key) >> 47, overlapped);
				break;
			case type::scheduler: {
				auto task = reinterpret_cast<io_complet_port::task*>(overlapped);
				if (false == task->execute())
					library::_thread_local::pool<io_complet_port::task>::instance().deallocate(task);
				else
					_scheduler_queue.emplace(task);
			} break;
			}
		}
	}
	inline void scheduler(void) noexcept {
		library::priority_queue<task*> ready_queue;
		auto wait = INFINITE;
		while (_scheduler_queue.wait(wait)) {
			while (!_scheduler_queue.empty())
				ready_queue.emplace(_scheduler_queue.pop());
			wait = INFINITE;
			auto time = library::time_get_time();
			while (!ready_queue.empty()) {
				if (auto top = ready_queue.top(); time < top->_time) {
					wait = top->_time - time;
					break;
				}
				else {
					ready_queue.pop();
					_complet_port.post_queue_state(0, 0xFF00000000000000ULL & (static_cast<unsigned long long>(type::scheduler) << 56), reinterpret_cast<OVERLAPPED*>(top));
				}
			}
		}
	}
};