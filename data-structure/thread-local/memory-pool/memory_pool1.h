#pragma once
#include "../../../design-pettern/thread-local/singleton/singleton.h"
#include "../../lockfree/stack/stack.h"
#include "../../pair/pair.h"

namespace data_structure::_thread_local {
	template<typename type, size_t bucket_size = 1024>
	class memory_pool final : public design_pattern::_thread_local::singleton<memory_pool<type, bucket_size>> {
	private:
		friend class design_pattern::_thread_local::singleton<memory_pool<type, bucket_size>>;
		using size_type = unsigned int;
		struct bucket final {
			inline explicit bucket(void) noexcept = delete;
			inline explicit bucket(bucket const&) noexcept = delete;
			inline explicit bucket(bucket&&) noexcept = delete;
			inline auto operator=(bucket const&) noexcept = delete;
			inline auto operator=(bucket&&) noexcept = delete;
			inline ~bucket(void) noexcept = delete;
			size_type _count = 0;
			type _array[bucket_size];
		};
		inline static consteval size_type power_of_two(size_type number, size_type square = 1) noexcept {
			return square >= number ? square : power_of_two(number, square << 1);
		}
		static constexpr size_type _align = power_of_two(sizeof(bucket));
		class stack final {
		private:
			struct node final {
				inline explicit node(void) noexcept = delete;
				inline explicit node(node const&) noexcept = delete;
				inline explicit node(node&&) noexcept = delete;
				inline auto operator=(node const&) noexcept = delete;
				inline auto operator=(node&&) noexcept = delete;
				inline ~node(void) noexcept = delete;
				node* _next;
				bucket* _value;
			};
		public:
			inline explicit stack(void) noexcept
				: _head(0) {
			}
			inline explicit stack(stack const& rhs) noexcept = delete;
			inline explicit stack(stack&& rhs) noexcept = delete;
			inline auto operator=(stack const& rhs) noexcept -> stack & = delete;
			inline auto operator=(stack&& rhs) noexcept -> stack & = delete;
			inline ~stack(void) noexcept {
				//bucket* head = reinterpret_cast<bucket*>(0x00007FFFFFFFFFFFULL & _head);
				//while (nullptr != head) {
				//	bucket* next = head->_next;
				//	_memory_pool.deallocate(*head);
				//	head = next;
				//}
			};
		public:
			inline void push(bucket* value) noexcept {
				node* current = &_memory_pool.allocate();
				current->_value = value;

				for (;;) {
					unsigned long long head = _head;
					current->_next = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
					unsigned long long next = reinterpret_cast<unsigned long long>(current) + (0xFFFF800000000000ULL & head);
					if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head))
						break;
				}
			}
			inline auto pop(void) noexcept -> bucket* {
				for (;;) {
					unsigned long long head = _head;
					node* address = reinterpret_cast<node*>(0x00007FFFFFFFFFFFULL & head);
					if (nullptr == address) {
						bucket* result = reinterpret_cast<bucket*>(_aligned_malloc(sizeof(bucket), _align));
						result->_count = 0;
						return result;
					}
					unsigned long long next = reinterpret_cast<unsigned long long>(address->_next) + (0xFFFF800000000000ULL & head) + 0x0000800000000000ULL;
					if (head == _InterlockedCompareExchange(reinterpret_cast<unsigned long long volatile*>(&_head), next, head)) {
						bucket* result = address->_value;
						_memory_pool.deallocate(*address);
						return result;
					}
				}
			}
		private:
			unsigned long long _head;
			lockfree::memory_pool<node> _memory_pool;
		};
	private:
		inline explicit memory_pool(void) noexcept = default;
		inline explicit memory_pool(memory_pool const& rhs) noexcept = delete;
		inline explicit memory_pool(memory_pool&& rhs) noexcept = delete;
		inline auto operator=(memory_pool const& rhs) noexcept -> memory_pool & = delete;
		inline auto operator=(memory_pool&& rhs) noexcept -> memory_pool & = delete;
		inline ~memory_pool(void) noexcept {
		};
	public:
		template<typename... argument>
		inline auto allocate(argument&&... arg) noexcept -> type& {
			if (bucket_size == _index) {
				_bucket = _stack.pop();
				_index = 0;
			}
			type& value = _bucket->_array[_index++];
			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					::new(reinterpret_cast<void*>(&value)) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
				value = type(std::forward<argument>(arg)...);
			return value;
		}
		inline void deallocate(type& value) noexcept {
			if constexpr (std::is_destructible_v<type> && !std::is_trivially_destructible_v<type>)
				value.~type();

			bucket* bucket_ = reinterpret_cast<bucket*>(reinterpret_cast<uintptr_t>(&value) & ~(uintptr_t)(_align - 1));
			if (bucket_size == _InterlockedIncrement(&bucket_->_count))
				_stack.push(bucket_);
		}
	private:
		bucket* _bucket;
		size_type _index = bucket_size;
		inline static stack _stack;
	};
}