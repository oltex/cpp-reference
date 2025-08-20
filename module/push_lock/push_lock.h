#pragma once
#include <Windows.h>
#include "library/memory.h"
#include "library/system/interlock.h"
#include <cassert>
#include <intrin.h>

class push_lock final {
	inline static constexpr unsigned long long EX_PUSH_LOCK_LOCK_V = 0x0;
	inline static constexpr unsigned long long EX_PUSH_LOCK_LOCK = 0x1000000000000000ULL;
	inline static constexpr unsigned long long EX_PUSH_LOCK_WAITING = 0x2000000000000000ULL;
	inline static constexpr unsigned long long EX_PUSH_LOCK_WAKING = 0x4000000000000000ULL;
	inline static constexpr unsigned long long EX_PUSH_LOCK_MULTIPLE_SHARED = 0x8000000000000000ULL;
	inline static constexpr unsigned long long EX_PUSH_LOCK_SHARE = 0x0FFFFFFFFFFFFFFFULL;
	inline static constexpr unsigned long long EX_PUSH_LOCK_SHARE_INC = 0x1ULL;
	inline static constexpr unsigned long long EX_PUSH_LOCK_PTR_BITS = 0xf000000000000000ULL;

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
	unsigned long long _head;


	inline static constexpr long EX_PUSH_LOCK_FLAGS_EXCLUSIVE = 0x1;
	inline static constexpr long EX_PUSH_LOCK_FLAGS_SPINNING_V = 0x1;
	inline static constexpr long EX_PUSH_LOCK_FLAGS_SPINNING = 0x2;
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
	struct node {
		//union {
		//	KGATE WakeGate;
		//	KEVENT WakeEvent;
		//};
		node* _next;
		node* _last;
		node* _prev;
		long _share;
		long _flag;
	};
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
			if (0 == (head & EX_PUSH_LOCK_LOCK)) { //���� �������� ������
				if (head == library::interlock_compare_exhange(_head, head + EX_PUSH_LOCK_LOCK, head)) //���� �ø�
					break;
			}
			else { //���� ����������
				auto current = library::allocate<node>();
				current->_flag = EX_PUSH_LOCK_FLAGS_EXCLUSIVE | EX_PUSH_LOCK_FLAGS_SPINNING;
				current->_prev = nullptr;
				unsigned long long next;
				bool optimize = false;

				if (0 == (head & EX_PUSH_LOCK_WAITING)) { //��⿭�� ������
					current->_last = current;
					current->_share = static_cast<long>(head & EX_PUSH_LOCK_SHARE);
					if (1 >= current->_share) { //���� �������� �������� 1�� ���ϸ�
						current->_share = 0;												// �̺κ� ���ؾȰ� ���� ī��Ʈ�� 1�̸� 1�� ������� ���ҽ��Ѽ� 0�Ǹ� �긦 Ǭ�� �̷��žƴѰ�?
						next = EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_LOCK | reinterpret_cast<unsigned long long>(current);
					}
					else //���� �������� �������� 2�� �̻��̸�
						next = EX_PUSH_LOCK_MULTIPLE_SHARED | EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_LOCK | reinterpret_cast<unsigned long long>(current);
				}
				else { //��⿭�� ������
					current->_last = nullptr;
					current->_next = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);
					current->_share = 0; //���� WATING �� �ִ��� �𸣰��� / LOCK�� �𸣰ڴ°� ��⿭�� ������ LOCK�� �翬�� �ִ°� �ƴѰǰ�? ���� ��⿭�� �糢�� ���� ����?
					next = (EX_PUSH_LOCK_WAKING | EX_PUSH_LOCK_WAITING | EX_PUSH_LOCK_LOCK | reinterpret_cast<unsigned long long>(current)) | (head & EX_PUSH_LOCK_PTR_BITS);

					if (0 == (head & EX_PUSH_LOCK_WAKING)) //���� ���� waking�� ������ ���� waiking�� ������ �ƴ�
						optimize = true; // �ٵ� �ƴϿ����� ���� waking�� �������� ���� �����״� �ʿ� ����? // �ٵ� ��밡 �����ÿ� ���µ� ���� ���߿� �ִ°Ŷ�� ���� �Ȼ����� ���� �ʳ�?
				}

				if (head == library::interlock_compare_exhange(_head, next, head)) {
					//if (true == optimize)
						// optimize();
					break;
				}
			}
		}
	}
	inline void acquire_share(void) noexcept {
		for (;;) {
			auto head = _head;
			unsigned long long next;
			if (0 == (head & EX_PUSH_LOCK_LOCK) || (0 == (head & EX_PUSH_LOCK_WAITING) && 0 < (head & EX_PUSH_LOCK_SHARE))) { //���� �ƹ��� ���� �ʾҰų�, ���� ������ ���� ī��Ʈ �ִµ� ������ ������ �ʾҰų�
				if (0 != (head & EX_PUSH_LOCK_WAITING)) //���� ���µ� �������� �ִ� ���? �̰� ���Ҹ���
					next = head + EX_PUSH_LOCK_LOCK;
				else
					next = head + EX_PUSH_LOCK_SHARE_INC | EX_PUSH_LOCK_LOCK; // ���� ���ų�, ���� �ְ� ����ī���Ͱ� �ְų� �Ҷ� ����
				if (head == library::interlock_compare_exhange(_head, next, head))
					break;
			}
			else {
				auto current = library::allocate<node>();
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
					//if (true == optimize)
						// optimize();
					break;
				}

			}
		}
	}
	inline void release_exclusive(void) noexcept {
		for (;;) {
			auto head = _head;
			unsigned long long next;
			if (0 == (head & EX_PUSH_LOCK_WAITING) || 0 != (head & EX_PUSH_LOCK_WAKING)) {
				next = head - EX_PUSH_LOCK_LOCK;
				if (head == library::interlock_compare_exhange(_head, next, head))
					break;
			}
			else {
				next = head - EX_PUSH_LOCK_LOCK + EX_PUSH_LOCK_WAKING;
				if (head == library::interlock_compare_exhange(_head, next, head)) {
					//ExfWakePushLock(next);
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

		if (0 != (head & EX_PUSH_LOCK_MULTIPLE_SHARED)) { //�������� ��Ƽ�� ������ �ִ�. 
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
				if (head == library::interlock_compare_exhange(_head, next, head))
					//ExfWakePushLock(next);
					return;
			}
			head = _head;
		}
	}

	inline void wake(unsigned long long head) noexcept {
		for (;;) {
			if (0 != (head & EX_PUSH_LOCK_LOCK))
				__debugbreak();
			/*while (OldValue.Locked) {
				NewValue.Value = OldValue.Value - EX_PUSH_LOCK_WAKING;
				ASSERT(!NewValue.Waking);
				ASSERT(NewValue.Locked);
				ASSERT(NewValue.Waiting);
				if ((NewValue.Ptr = InterlockedCompareExchangePointer(&PushLock->Ptr,
					NewValue.Ptr,
					OldValue.Ptr)) == OldValue.Ptr) {
					return;
				}
				OldValue = NewValue;
			}*/

			auto current = reinterpret_cast<node*>(head & EX_PUSH_LOCK_SHARE);
			auto first = current;
			node* prev;
			while (1) {
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
			}
			else {
				first->_last = prev;
				current->_prev = nullptr;
				library::interlock_and(_head, ~EX_PUSH_LOCK_WAKING);
				break;
			}
		}
	}

};