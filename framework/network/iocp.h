#pragma once
#include "library/system/inputoutput_complet_port.h"
#include "library/pattern/singleton.h"
#include "library/system/thread.h"
#include "library/container/vector.h"

namespace framework {
	class iocp final : public library::singleton<iocp, true> {
		friend class library::singleton<iocp, true>;
		using size_type = unsigned int;
		enum class task : unsigned char {
			close = 0, 
		};
		library::inputoutput_complet_port _complet_port;
		library::vector<library::thread> _worker_thread;
	public:
		class object {
		public:
			inline virtual void worker(bool result, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept = 0;
		};

		inline explicit iocp(size_type concurrent, size_type worker) noexcept {
			_complet_port.create(concurrent);
			for (size_type index = 0; index < worker; ++index)
				_worker_thread.emplace_back(&iocp::worker, 0, this);
		}
		inline explicit iocp(iocp const&) noexcept = delete;
		inline explicit iocp(iocp&&) noexcept = delete;
		inline auto operator=(iocp const&) noexcept -> iocp & = delete;
		inline auto operator=(iocp&&) noexcept -> iocp & = delete;
		inline ~iocp(void) noexcept {
			for (size_type index = 0; index < _worker_thread.size(); ++index)
				_complet_port.post_queue_state(0, 0, nullptr);
			HANDLE handle[128];
			for (unsigned int index = 0; index < _worker_thread.size(); ++index)
				handle[index] = _worker_thread[index].data();
			library::handle::wait_for_multiple(_worker_thread.size(), handle, true, INFINITE);
			_worker_thread.clear();
			_complet_port.close();
		};

		inline void connect(object& object, library::socket& socket, uintptr_t key) noexcept {
			_complet_port.connect(socket, (key << 47) | reinterpret_cast<uintptr_t>(&object));
		}
		inline void post(object& object, unsigned long transferred, uintptr_t key, OVERLAPPED* overlapped) noexcept {
			_complet_port.post_queue_state(transferred, (key << 47) | reinterpret_cast<uintptr_t>(&object), overlapped);
		}
	private:
		inline void worker(void) noexcept {
			for (;;) {
				auto [result, transferred, key, overlapped] = _complet_port.get_queue_state(INFINITE);
				if (0 == key)
					return;
				else
					reinterpret_cast<object*>(0x00007FFFFFFFFFFF & key)->worker(result, transferred, (key >> 47), overlapped);
			}
		}
	};
}


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