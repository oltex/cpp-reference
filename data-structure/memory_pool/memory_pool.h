#pragma once
#include <memory>

namespace data_structure {
	template<typename type>
	class memory_pool final {
	private:
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
			while (nullptr != _head) {
#pragma warning(suppress: 6001)
				node* next = _head->_next;
				free(_head);
				_head = next;
			}
		}
	public:
		template<typename... argument>
		inline auto allocate(argument&&... arg) noexcept -> type& {
			node* current;
			if (nullptr == _head)
				current = static_cast<node*>(malloc(sizeof(node)));
			else {
				current = _head;
				_head = current->_next;
			}

			if constexpr (std::is_class_v<type>) {
				if constexpr (std::is_constructible_v<type, argument...>)
					new(&current->_value) type(std::forward<argument>(arg)...);
			}
			else if constexpr (1 == sizeof...(arg))
#pragma warning(suppress: 6011)
				current->_value = type(std::forward<argument>(arg)...);
			return current->_value;
		}
		inline void deallocate(type& value) noexcept {
			if constexpr (!std::is_trivially_destructible_v<type>)
				value.~type();
			reinterpret_cast<node*>(&value)->_next = _head;
			_head = reinterpret_cast<node*>(&value);
		}
	private:
		node* _head = nullptr;
	};
}



//namespace data_structure {
//	template<size_t _size, size_t _align>
//	class memory_pool final {
//	private:
//		using size_type = unsigned int;
//		using byte = unsigned char;
//		//inline static constexpr size_type _size = sizeof(_size) < alignof(void*) ? alignof(void*) : alignof(_align);
//		inline static constexpr size_type _align = alignof(_align) < alignof(void*) ? alignof(void*) : alignof(_align);
//		struct node final {
//			node* _next;
//		};
//		struct block final {
//			block* _next;
//			node* _array;
//		};
//	public:
//		inline explicit memory_pool(void) noexcept = default;
//		//not implemented
//		inline explicit memory_pool(memory_pool const& rhs) noexcept;
//		//not implemented
//		inline explicit memory_pool(memory_pool&& rhs) noexcept;
//		//not implemented
//		inline auto operator=(memory_pool const& rhs) noexcept -> memory_pool&;
//		//not implemented
//		inline auto operator=(memory_pool&& rhs) noexcept -> memory_pool&;
//		inline ~memory_pool(void) noexcept {
//			while (nullptr != _block) {
//				block* next = _block->_next;
//				_aligned_free(_block->_array);
//				free(_block);
//				_block = next;
//			}
//		}
//	public:
//		inline auto allocate(void) noexcept -> void* {
//			if (nullptr == _head) {
//				size_type capacity = static_cast<size_type>(_capacity * 1.5f);
//				if (1 >= _capacity)
//					capacity++;
//				reserve(capacity);
//			}
//			void* ret = reinterpret_cast<void*>(_head);
//			_head = _head->_next;
//			return ret;
//		}
//		inline void deallocate(void* const value) noexcept {
//			reinterpret_cast<node*>(value)->_next = _head;
//			_head = reinterpret_cast<node*>(value);
//		}
//	public:
//		inline void reserve(size_type capacity) noexcept {
//			capacity -= _capacity;
//
//			byte* arr = static_cast<byte*>(_aligned_malloc(_size * capacity, _align));
//			reinterpret_cast<node*>(arr + _byte * (capacity - 1))->_next = _head;
//			_head = reinterpret_cast<node*>(arr);
//
//			block* cur = static_cast<block*>(malloc(sizeof(block)));
//			cur->_array = reinterpret_cast<node*>(arr);
//			cur->_next = _block;
//			_block = cur;
//
//			for (size_type index = 0; index < (capacity - 1); ++index, arr += _byte)
//				reinterpret_cast<node*>(arr)->_next = reinterpret_cast<node*>(arr + _byte);
//
//			_capacity += capacity;
//		}
//	private:
//		node* _head = nullptr;
//		block* _block = nullptr;
//		size_type _capacity = 0;
//	};
//}