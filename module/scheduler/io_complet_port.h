#pragma once
#include "library/system/inputoutput_complet_port.h"
#include "library/system/thread.h"
#include "library/container/vector.h"
#include "library/pattern/singleton.h"

class io_complet_port : public library::singleton<io_complet_port, true> {
	friend class library::singleton<io_complet_port, true>;
	using size_type = unsigned int;
	enum class type : unsigned char {
		close = 0
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
			if (0 == key)
				return;
			else
				reinterpret_cast<io_complet_object*>(0x00007FFFFFFFFFFF & key)->worker(result, transferred, (key >> 47), overlapped);
		}
	}
};