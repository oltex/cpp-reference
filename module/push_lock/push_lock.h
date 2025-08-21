#pragma once
#include <Windows.h>
#include "library/memory.h"
#include "library/system/interlock.h"
#include "library/system/fence.h"
#include "library/system/event.h"
#include <cassert>
#include <intrin.h>


namespace library {
	//struct log final {
	//	unsigned long _thread_id;
	//	wchar_t const* _action;
	//	void* _head = 0;
	//	void* _next = 0;
	//};
	//auto order = _InterlockedIncrement(&_order) % 10000000;
	//_log[order]._thread_id = GetCurrentThreadId();
	//_log[order]._action = L"push";
	//_log[order]._head = (void*)head;
	//_log[order]._next = (void*)next;
	class push_lock final {
		//volatile unsigned int _order = 0;
		//library::log* _log = new library::log[10000000]{};
		inline static constexpr long EX_PUSH_LOCK_FLAGS_EXCLUSIVE = 0x1;
		inline static constexpr long EX_PUSH_LOCK_FLAGS_SPINNING_V = 0x1;
		inline static constexpr long EX_PUSH_LOCK_FLAGS_SPINNING = 0x2;
		struct node {
			node* _next;
			node* _last;
			node* _prev;
			long _share;
			long _flag;
			library::event _event;
		};
		inline static constexpr unsigned long long EX_PUSH_LOCK_LOCK_V = 0x0;
		inline static constexpr unsigned long long EX_PUSH_LOCK_LOCK = 0x1000000000000000ULL;
		inline static constexpr unsigned long long EX_PUSH_LOCK_WAITING = 0x2000000000000000ULL;
		inline static constexpr unsigned long long EX_PUSH_LOCK_WAKING = 0x4000000000000000ULL;
		inline static constexpr unsigned long long EX_PUSH_LOCK_MULTIPLE_SHARED = 0x8000000000000000ULL;
		inline static constexpr unsigned long long EX_PUSH_LOCK_SHARE = 0x0FFFFFFFFFFFFFFFULL;
		inline static constexpr unsigned long long EX_PUSH_LOCK_SHARE_INC = 0x1ULL;
		inline static constexpr unsigned long long EX_PUSH_LOCK_PTR_BITS = 0xf000000000000000ULL;
		unsigned long long _head;
	public:
		inline explicit push_lock(void) noexcept
			: _head(0) {
		};
		inline explicit push_lock(push_lock const&) noexcept = delete;
		inline explicit push_lock(push_lock&&) noexcept = delete;
		inline auto operator=(push_lock const&) noexcept -> push_lock & = delete;
		inline auto operator=(push_lock&&) noexcept -> push_lock & = delete;
		inline ~push_lock(void) noexcept = default;

		inline void acquire_exclusive(void) noexcept {
			for (;;) {
				auto head = _head;
				if (0 == (head & EX_PUSH_LOCK_LOCK)) { //락이 켜져있지 않으면
					if (head == library::interlock_compare_exhange(_head, head + EX_PUSH_LOCK_LOCK, head)) {//락을 올림
						//auto order = _InterlockedIncrement(&_order) % 10000000;
						//_log[order]._thread_id = GetCurrentThreadId();
						//_log[order]._action = L"acquire_exclusive 1";
						//_log[order]._head = (void*)head;
						//_log[order]._next = (void*)(head + EX_PUSH_LOCK_LOCK);
						break;
					}
				}
				else { //락이 켜져있으면
					auto current = library::allocate<node>();
					library::construct(current->_event, false, false);
					current->_flag = EX_PUSH_LOCK_FLAGS_EXCLUSIVE/* | EX_PUSH_LOCK_FLAGS_SPINNING*/;
					current->_prev = nullptr;
					unsigned long long next;
					bool optimize = false;

					if (0 == (head & EX_PUSH_LOCK_WAITING)) { //대기열이 없으면
						current->_last = current;
						current->_share = static_cast<long>(head & EX_PUSH_LOCK_SHARE);
						if (1 >= current->_share) { //먼저 진행중인 공유락이 1개 이하면
							current->_share = 0;												// 이부분 이해안감 쉐어 카운트가 1이면 1로 해줘야지 감소시켜서 0되면 얘를 푼다 이런거아닌가?
							next = EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_LOCK | reinterpret_cast<unsigned long long>(current);
						}
						else //먼저 진행중인 공유략이 2개 이상이면
							next = EX_PUSH_LOCK_MULTIPLE_SHARED | EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_LOCK | reinterpret_cast<unsigned long long>(current);
					}
					else { //대기열이 있으면
						current->_last = nullptr;
						current->_next = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);
						current->_share = 0; //굳이 WATING 왜 넣는지 모르겠음 / LOCK도 모르겠는게 대기열이 있으면 LOCK이 당연히 있는게 아닌건가? 누가 대기열을 재끼고 먼저 들어간다?
						next = (EX_PUSH_LOCK_WAKING | EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_LOCK | reinterpret_cast<unsigned long long>(current)) | (head & EX_PUSH_LOCK_PTR_BITS);

						if (0 == (head & EX_PUSH_LOCK_WAKING)) //과거 값이 waking이 였으면 내가 waiking을 먹은게 아님
							optimize = true; // 근데 아니였으면 내가 waking을 먹은거임 굳이 나한테는 필요 없나? // 근데 상대가 웨이팅에 들어갔는데 내가 나중에 넣는거라면 내껄 안뺄수도 있지 않나?
					}

					if (head == library::interlock_compare_exhange(_head, next, head)) {
						//auto order = _InterlockedIncrement(&_order) % 10000000;
						//_log[order]._thread_id = GetCurrentThreadId();
						//_log[order]._action = L"acquire_exclusive 2";
						//_log[order]._head = (void*)head;
						//_log[order]._next = (void*)next;

						if (true == optimize)
							push_lock::optimize(next);

						//for (auto index = 0; index < 66; ++index) {
						//	if (0 == (current->_flag & EX_PUSH_LOCK_FLAGS_SPINNING))
						//		break;
						//	library::pause();
						//}
						//if (library::interlock_bit_test_and_reset(current->_flag, EX_PUSH_LOCK_FLAGS_SPINNING_V))
						current->_event.wait_for_single(INFINITE);
					}
					current->_event.close();
					library::deallocate(current);
				}
			}
		}
		inline void acquire_share(void) noexcept {
			for (;;) {
				auto head = _head;
				unsigned long long next;
				if (0 == (head & EX_PUSH_LOCK_LOCK) || (0 == (head & EX_PUSH_LOCK_WAITING) && 0 < (head & EX_PUSH_LOCK_SHARE))) { //락을 아무도 잡지 않았거나, 락은 잡히고 쉐어 카운트 있는데 스택이 생기진 않았거나
					if (0 != (head & EX_PUSH_LOCK_WAITING)) //락이 없는데 웨이팅은 있는 경우? 이게 뭔소릴까
						next = head + EX_PUSH_LOCK_LOCK;
					else
						next = head + EX_PUSH_LOCK_SHARE_INC | EX_PUSH_LOCK_LOCK; // 락이 없거나, 락은 있고 쉐어카운터가 있거나 할때 증가
					if (head == library::interlock_compare_exhange(_head, next, head))
						break;
				}
				else {
					auto current = library::allocate<node>();
					library::construct(current->_event, false, false);
					current->_flag = EX_PUSH_LOCK_FLAGS_SPINNING;
					current->_share = 0;
					current->_prev = nullptr;
					bool optimize = false;

					if (0 == (head & EX_PUSH_LOCK_WAITING)) {
						current->_last = current;
						next = EX_PUSH_LOCK_WAITING | reinterpret_cast<unsigned long long>(current) | (head & EX_PUSH_LOCK_PTR_BITS);
					}
					else {
						current->_last = nullptr;
						current->_next = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);
						next = EX_PUSH_LOCK_WAKING | EX_PUSH_LOCK_WAITING | reinterpret_cast<unsigned long long>(current) | (head & (EX_PUSH_LOCK_LOCK | EX_PUSH_LOCK_MULTIPLE_SHARED));

						if (0 == (head & EX_PUSH_LOCK_WAKING))
							optimize = true;
					}

					if (head == library::interlock_compare_exhange(_head, next, head)) {
						if (true == optimize)
							push_lock::optimize(next);

						for (auto index = 0; index < 66; ++index) {
							if (0 == (current->_flag & EX_PUSH_LOCK_FLAGS_SPINNING))
								break;
							library::pause();
						}
						if (library::interlock_bit_test_and_reset(current->_flag, EX_PUSH_LOCK_FLAGS_SPINNING_V))
							current->_event.wait_for_single(INFINITE);
					}
					library::deallocate(current);
				}
			}
		}
		inline void release_exclusive(void) noexcept {
			for (;;) {
				auto head = _head;
				unsigned long long next;
				if (0 == (head & EX_PUSH_LOCK_WAITING) || 0 != (head & EX_PUSH_LOCK_WAKING)) {
					next = head - EX_PUSH_LOCK_LOCK;
					if (head == library::interlock_compare_exhange(_head, next, head)) {
						//auto order = _InterlockedIncrement(&_order) % 10000000;
						//_log[order]._thread_id = GetCurrentThreadId();
						//_log[order]._action = L"release_exclusive 1";
						//_log[order]._head = (void*)head;
						//_log[order]._next = (void*)next;
						break;
					}
				}
				else {
					next = head - EX_PUSH_LOCK_LOCK + EX_PUSH_LOCK_WAKING;
					if (head == library::interlock_compare_exhange(_head, next, head)) {
						wake(next);
						break;
					}
				}
			}
		}
		inline void release_share(void) noexcept {
			auto head = _head;
			unsigned long long next;
			while (0 == (head & EX_PUSH_LOCK_WAITING)) {
				if (1 < (head & EX_PUSH_LOCK_SHARE))
					next = head - EX_PUSH_LOCK_SHARE_INC;
				else
					next = 0;
				if (head == library::interlock_compare_exhange(_head, next, head))
					return;
				head = _head;
			}

			if (0 != (head & EX_PUSH_LOCK_MULTIPLE_SHARED)) { //독점락이 멀티를 가지고 있다. 
				auto current = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);

				for (;;) {
					auto last = current->_last;
					if (last != NULL) {
						current = last;
						break;
					}
					current = current->_next;
				}

				if (0 < library::interlock_decrement(current->_share))
					return;
			}

			for (;;) {
				if (0 != (head & EX_PUSH_LOCK_WAKING)) {
					next = head & ~(EX_PUSH_LOCK_LOCK | EX_PUSH_LOCK_MULTIPLE_SHARED);
					if (head == library::interlock_compare_exhange(_head, next, head))
						return;
				}
				else {
					next = EX_PUSH_LOCK_WAKING | (head & ~(EX_PUSH_LOCK_LOCK | EX_PUSH_LOCK_MULTIPLE_SHARED));
					if (head == library::interlock_compare_exhange(_head, next, head)) {
						wake(next);
						return;
					}
				}
				head = _head;
			}
		}
		inline void optimize(unsigned long long head) noexcept {
			for (;;) {
				if (0 == (head & EX_PUSH_LOCK_LOCK)) {
					wake(head);
					break;
				}

				auto current = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);
				auto first = current;
				for (;;) {
					auto last = current->_last;
					if (nullptr != last) {
						current = last;
						break;
					}
					auto prev = current;
					current = current->_next;
					current->_prev = prev;
				}

				unsigned long long next = head - EX_PUSH_LOCK_WAKING;
				if (head == library::interlock_compare_exhange(_head, next, head))
					break;
				head = _head;
			}
		}
		inline void wake(unsigned long long head) noexcept {
			unsigned long long next;
			node* current;
			for (;;) {
				while (0 != (head & EX_PUSH_LOCK_LOCK)) {
					next = head - EX_PUSH_LOCK_WAKING;
					if (head == library::interlock_compare_exhange(_head, next, head)) {
						return;
					}
					head = _head;
				}

				current = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);
				auto first = current;
				node* prev;
				for (;;) {
					auto last = current->_last;
					if (nullptr != last) {
						current = last;
						break;
					}
					prev = current;
					current = current->_next;
					current->_prev = prev;
				}

				if (!(current->_flag & EX_PUSH_LOCK_FLAGS_EXCLUSIVE && nullptr != (prev = current->_prev))) {
					unsigned long long next = 0;
					if (head == library::interlock_compare_exhange(_head, next, head))
						break;
					head = _head;
				}
				else {
					first->_last = prev;
					current->_prev = nullptr;
					library::interlock_and(_head, ~EX_PUSH_LOCK_WAKING);
					break;
				}
			}

			//if(nullptr != current->_prev) //DPC는 못올리지만 스레드 우선순위 최상위로 해두는게 좋을수도
				//dispatch level up

			for (;;) {
				auto prev = current->_prev;
				if (false == library::interlock_bit_test_and_reset(current->_flag, EX_PUSH_LOCK_FLAGS_SPINNING_V))
					current->_event.set();
				current = prev;
				if (nullptr == current)
					break;
			}

			//if(nullptr != current->_prev)
				//dispatch level down
		}
	};
}

//struct EX_PUSH_LOCK {
//	union {
//		struct {
//			ULONG_PTR Locked : 1;
//			ULONG_PTR Waiting : 1;
//			ULONG_PTR Waking : 1;
//			ULONG_PTR MultipleShared : 1;
//			ULONG_PTR Shared : sizeof(ULONG_PTR) * 8 - 4;
//		};
//		ULONG_PTR Value;
//		PVOID Ptr;
//	};
//};
//EX_PUSH_LOCK _head;

		//struct DECLSPEC_ALIGN(16) EX_PUSH_LOCK_WAIT_BLOCK {
		//	//union {
		//	//	KGATE WakeGate;
		//	//	KEVENT WakeEvent;
		//	//};
		//	EX_PUSH_LOCK_WAIT_BLOCK* Next;
		//	EX_PUSH_LOCK_WAIT_BLOCK* Last;
		//	EX_PUSH_LOCK_WAIT_BLOCK* Previous;
		//	LONG ShareCount;
		//	LONG Flags;
		//};