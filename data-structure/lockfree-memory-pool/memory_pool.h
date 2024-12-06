#pragma once
#include <memory>
#include <Windows.h>

namespace data_structure::lockfree {
	template<typename type>
	class memory_pool final {
	private:
		using pointer_type = unsigned long long;
		union pointer final {
			void* _data;
			struct {
				pointer_type _address : 47;
				pointer_type _count : 17;
			};
		};
		union node final {
			node* _next;
			type _value;
		};
	public:
		inline explicit memory_pool(void) noexcept = default;
		inline explicit memory_pool(memory_pool const&) noexcept = delete;
		inline explicit memory_pool(memory_pool&& rhs) noexcept
			: _head(rhs._head) {
			rhs._head = nullptr;
		};
		inline auto operator=(memory_pool const&) noexcept = delete;
		inline auto operator=(memory_pool&& rhs) noexcept {
			_head = rhs._head;
			rhs._head = nullptr;
		}
		inline ~memory_pool(void) noexcept {
			//			while (nullptr != _head) {
			//#pragma warning(suppress: 6001)
			//				node* next = _head->_next;
			//				free(_head);
			//				_head = next;
			//			}
		}
	public:
		template<typename... argument>
		inline auto allocate(argument&&... arg) noexcept -> type& {
			pointer current;
			for (;;) {
				current = _head;
				if (nullptr == reinterpret_cast<node*>(current._address)) {
					current._address = reinterpret_cast<pointer_type>(malloc(sizeof(node)));
					break;
				}
				//reinterpret_cast<node*>(current._address)->_next._count = 1 + current._count;
				pointer next;
				next._address = reinterpret_cast<pointer_type>(reinterpret_cast<node*>(current._address)->_next);
				next._count = 1 + current._count;
				if (current._data != _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head._data), next._data, current._data))
					break;
			}

			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					::new(reinterpret_cast<void*>(&current->_value)) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
				current->_value = type(std::forward<argument>(arg)...);
			return reinterpret_cast<node*>(current._address)->_value;
		}
		inline void deallocate(type& value) noexcept {
			if constexpr (!std::is_trivially_destructible_v<type>)
				value.~type();

			do {
				pointer current = _head;
				reinterpret_cast<node&>(value)._next = reinterpret_cast<node*>(current._address);
				pointer next;
				next._address = reinterpret_cast<pointer_type>(&value);
				next._count = 1 + current._count;
			} while (reinterpret_cast<node*>(&value)->_next != _InterlockedCompareExchangePointer(reinterpret_cast<void* volatile*>(&_head._data), next._data, current._data));
		}
	public:
		inline void test(void) noexcept {
			//for (int i = 0; i < 10000; ++i) {
			//	node* current = static_cast<node*>(malloc(sizeof(node)));
			//	current->_value = 10;
			//	current->_next = _head;
			//	_head = current;
			//}
		}
	private:
		pointer _head{};
	};
}