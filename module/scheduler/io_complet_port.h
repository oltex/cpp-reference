#pragma once
#include "library/system/inputoutput_complet_port.h"
#include "library/system/thread.h"
#include "library/container/vector.h"
#include "library/pattern/singleton.h"

class io_complet_port : public library::singleton<io_complet_port, true> {
	friend class library::singleton<io_complet_port, true>;
	using size_type = unsigned int;
	enum class type : unsigned char {
		close = 0,
	};
	library::inputoutput_complet_port _complet_port;
	library::vector<library::thread> _worker_thread;
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

	inline explicit io_complet_port(size_type concurrent, size_type worker) noexcept {
		_complet_port.create(concurrent);
		for (size_type index = 0; index < worker; ++index)
			_worker_thread.emplace_back(&io_complet_port::worker, 0, this);
	}
	inline explicit io_complet_port(io_complet_port const&) noexcept = delete;
	inline explicit io_complet_port(io_complet_port&&) noexcept = delete;
	inline auto operator=(io_complet_port const&) noexcept -> io_complet_port & = delete;
	inline auto operator=(io_complet_port&&) noexcept -> io_complet_port & = delete;
	inline ~io_complet_port(void) noexcept {
		for (size_type index = 0; index < _worker_thread.size(); ++index)
			_complet_port.post_queue_state(0, 0, nullptr);
		HANDLE handle[128];
		for (unsigned int index = 0; index < _worker_thread.size(); ++index)
			handle[index] = _worker_thread[index].data();
		library::handle::wait_for_multiple(_worker_thread.size(), handle, true, INFINITE);
		_worker_thread.clear();
		_complet_port.close();
	};

	inline void connect(io_complet_object& object, library::socket& socket, uintptr_t key) noexcept {
		_complet_port.connect(socket, (key << 47) | reinterpret_cast<uintptr_t>(&object));
	}
	inline void post(io_complet_object& object, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept {
		_complet_port.post_queue_state(transferred, (key << 47) | reinterpret_cast<uintptr_t>(&object), overlapped);
	}
private:
	inline void worker(void) noexcept {
		for (;;) {
			auto [result, transferred, key, overlapped] = _complet_port.get_queue_state(INFINITE);
			if (type::close == static_cast<type>(0xFFFF800000000000 & key))
				return;
			else
				reinterpret_cast<io_complet_object*>(0x00007FFFFFFFFFFF & key)->worker(result, transferred, (key >> 47), overlapped);
		}
	}
};


//namespace framework {
//	class iocp final : public library::singleton<iocp, true> {
//		friend class library::singleton<iocp, true>;
//		using size_type = unsigned int;
//		enum class type : unsigned char {
//			close = 0,
//		};
//		library::inputoutput_complet_port _complet_port;
//		library::vector<library::thread> _worker_thread;
//
//		struct _task : public library::intrusive::share_pointer_hook<0> {
//			unsigned long _time;
//			std::function<int(void)> _function;
//
//			template <typename function, typename... argument>
//			inline explicit _task(function&& func, argument&&... arg) noexcept
//				:_function(std::bind(std::forward<function>(func), std::forward<argument>(arg)...)) {
//			};
//
//			template<size_t index>
//			inline void destructor(void) noexcept;
//			template<>
//			inline void destructor<0>(void) noexcept {
//				library::_thread_local::pool<_task>::instance().deallocate(this);
//			}
//		};
//		using task = library::intrusive::share_pointer<_task, 0>;
//		class queue final : public library::lockfree::queue<task, false> {
//		public:
//			inline explicit queue(void) noexcept = default;
//			inline explicit queue(queue const&) noexcept = delete;
//			inline explicit queue(queue&&) noexcept = delete;
//			inline auto operator=(queue const&) noexcept -> queue & = delete;
//			inline auto operator=(queue&&) noexcept -> queue & = delete;
//			inline ~queue(void) noexcept = default;
//
//			//inline void wake(void) noexcept {
//			//	library::wake_by_address_single();
//			//}
//			//inline bool wait(void* compare, unsigned long const wait_time) noexcept {
//			//	library::wait_on_address()
//			//	return system_component::wait_on_address::wait(&_size, compare, sizeof(size_type), wait_time);
//			//}
//		};
//		inline static auto less(task const& left, task const& right) {
//			return left->_time < right->_time;
//		}
//		bool _scheduler_active = 0;
//		queue _scheduler_queue;
//
//	public:
//		class object {
//		public:
//			inline virtual void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept = 0;
//		};
//
//		inline explicit iocp(size_type concurrent, size_type worker) noexcept {
//			_complet_port.create(concurrent);
//			for (size_type index = 0; index < worker; ++index)
//				_worker_thread.emplace_back(&iocp::worker, 0, this);
//		}
//		inline explicit iocp(iocp const&) noexcept = delete;
//		inline explicit iocp(iocp&&) noexcept = delete;
//		inline auto operator=(iocp const&) noexcept -> iocp & = delete;
//		inline auto operator=(iocp&&) noexcept -> iocp & = delete;
//		inline ~iocp(void) noexcept {
//			for (size_type index = 0; index < _worker_thread.size(); ++index)
//				_complet_port.post_queue_state(0, 0, nullptr);
//			HANDLE handle[128];
//			for (unsigned int index = 0; index < _worker_thread.size(); ++index)
//				handle[index] = _worker_thread[index].data();
//			library::handle::wait_for_multiple(_worker_thread.size(), handle, true, INFINITE);
//			_worker_thread.clear();
//			_complet_port.close();
//		};
//
//		inline void connect(object& object, library::socket& socket, uintptr_t key) noexcept {
//			_complet_port.connect(socket, (key << 47) | reinterpret_cast<uintptr_t>(&object));
//		}
//		inline void post(object& object, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept {
//			_complet_port.post_queue_state(transferred, (key << 47) | reinterpret_cast<uintptr_t>(&object), overlapped);
//		}
//		/*inline void do_create_function() noexcept {
//			if (0 == _scheduler._active) {
//				_InterlockedIncrement(&_scheduler._size);
//				if (0 == _scheduler._active) {
//					auto& memory_pool = data_structure::_thread_local::memory_pool<scheduler::function>::instance();
//					scheduler::task* task_(&memory_pool.allocate(std::forward<function>(func), std::forward<argument>(arg)...));
//					_complation_port.post_queue_state(0, static_cast<uintptr_t>(post_queue_state::excute_task), reinterpret_cast<OVERLAPPED*>(task_));
//				}
//				else
//					_InterlockedDecrement(&_scheduler._size);
//			}
//		}*/
//		template <typename function, typename... argument>
//		inline void func(function&& func, argument&&... arg) noexcept {
//			task task = library::_thread_local::pool<_task>::instance().allocate(std::forward<function>(func), std::forward<argument>(arg)...);
//			_scheduler_queue.emplace(task);
//		}
//	private:
//		inline void worker(void) noexcept {
//			for (;;) {
//				auto [result, transferred, key, overlapped] = _complet_port.get_queue_state(INFINITE);
//				if (0 == key)
//					return;
//				else
//					reinterpret_cast<object*>(0x00007FFFFFFFFFFF & key)->worker(result, transferred, (key >> 47), overlapped);
//			}
//		}
//		inline void schedule(void) noexcept {
//			library::priority_queue<task, less> _ready_queue;
//			for (;;) {
//
//			}
//		/*	unsigned long wait_time = INFINITE;
//			while (0 == _scheduler._active || 0 != _scheduler._size) {
//
//				if (_task_queue.wait(wait_time)) {
//					while (!_scheduler._task_queue.empty())
//						_ready_queue.push(&_scheduler._task_queue.pop());
//				}
//				wait_time = INFINITE;
//				unsigned long time = system_component::time::multimedia::get_time();
//				while (!_ready_queue.empty()) {
//					auto task_ = _ready_queue.top();
//					if (time < task_->_time) {
//						wait_time = static_cast<unsigned long>(task_->_time - time);
//						break;
//					}
//					_ready_queue.pop();
//					_complation_port.post_queue_state(0, static_cast<uintptr_t>(post_queue_state::excute_task), reinterpret_cast<OVERLAPPED*>(task_));
//				}
//			}*/
//		}
//	};
//}


//class scheduler final {
//public:
//	class task {
//	public:
//		template <typename function_, typename... argument>
//		inline explicit task(function_&& func, argument&&... arg) noexcept
//			: task(type::function), _function(std::bind(std::forward<function_>(func), std::forward<argument>(arg)...)) {
//		};
//		inline explicit task(task const&) noexcept = delete;
//		inline explicit task(task&&) noexcept = delete;
//		inline auto operator=(task const&) noexcept -> task & = delete;
//		inline auto operator=(task&&) noexcept -> task & = delete;
//		inline ~task(void) noexcept = default;
//
//		inline virtual bool excute(void) noexcept override {
//			for (;;) {
//				int time = _function();
//				switch (time) {
//				case 0:
//					break;
//				case -1:
//					return false;
//				default:
//					_time += time;
//					return true;
//				}
//			}
//		}
//	private:
//		std::function<int(void)> _function;
//	};
//	class task {
//	public:
//#pragma warning(suppress: 26495)
//		inline explicit task(type const type_) noexcept
//			: _type(type_), _time(system_component::time::multimedia::get_time()) {
//		};
//		inline explicit task(task const&) noexcept = delete;
//		inline explicit task(task&&) noexcept = delete;
//		inline auto operator=(task const&) noexcept -> task & = delete;
//		inline auto operator=(task&&) noexcept -> task & = delete;
//		inline ~task(void) noexcept = default;
//
//		inline virtual bool excute(void) noexcept = 0;
//
//		type _type;
//		unsigned long _time;
//	};
//}