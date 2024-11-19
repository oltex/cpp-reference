#pragma once
#include <memory>
#include <iostream>

constexpr auto array_size(unsigned char size) noexcept -> unsigned int {
	unsigned int bit = 0;
	while (size > 1) {
		size /= 2;
		bit++;
	}
	if ((1UL << bit) < size)
		bit++;
	return 12 - bit;
}

namespace data_structure {
	template<typename type>
	class allocator final {
	private:
		using size_type = unsigned int;
	private:
		class memory_pool final {
		private:
			struct node final {
				node* _next;
			};
			struct block final {
				block* _next;
				node* _array;
			};
		public:
			inline explicit memory_pool(void) noexcept {
				//std::cout << size << std::endl;
			}
		};
	public:
		inline explicit allocator(void) noexcept {
			constexpr int a = array_size(sizeof(type));
			unsigned long bit;
			_BitScanReverse64(&bit, sizeof(type));
			if ((1UL << bit) < sizeof(type))
				bit++;
			//for (auto index = 0; index < 13 - bit; ++index) {
				//new memory_pool(static_cast<size_type>(pow(2, index)) * sizeof(type));
			//}
		};
		inline explicit allocator(allocator const& rhs) noexcept;
		inline explicit allocator(allocator&& rhs) noexcept;
		inline auto operator=(allocator const& rhs) noexcept -> allocator&;
		inline auto operator=(allocator&& rhs) noexcept -> allocator&;
		inline ~allocator(void) noexcept = default;
	public:
		inline auto allocate(size_type const size) noexcept {
			size_type byte = sizeof(type) * size;
			if (byte > 4096)
				return reinterpret_cast<type*>(malloc(byte));

			unsigned long bit;
			_BitScanReverse64(&bit, byte);
			if ((1UL << bit) < byte)
				bit++;
		}
		inline void deallocate(type* const value, size_type const size) noexcept {
			size_type byte = sizeof(type) * size;
			if (byte > 4096) {
				free(value);
				return;
			}
			unsigned long bit;
			_BitScanReverse64(&bit, byte);
			bit += 1;
		}
	private:
		memory_pool _memory_pool[array_size(sizeof(type))];
	};
}


//namespace data_structure {
//	template<size_t _size, size_t _align>
//	class memory_pool final {
//	private:
//		using size_type = unsigned int;
//		using byte = unsigned char;
//		inline static constexpr size_type _size = sizeof(_size) < alignof(void*) ? alignof(void*) : alignof(_align);
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
//			byte* arr = static_cast<byte*>(_aligned_malloc(_byte * capacity, _align));
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



#pragma once
#include <memory>

namespace data_structure {
	template<size_t _size, size_t _align>
	class memory_pool final {
	private:
		using size_type = unsigned int;
		using byte = unsigned char;
		//inline static constexpr size_type _size = sizeof(_size) < alignof(void*) ? alignof(void*) : alignof(_align);
		inline static constexpr size_type _align = alignof(_align) < alignof(void*) ? alignof(void*) : alignof(_align);
		struct node final {
			node* _next;
		};
		struct block final {
			block* _next;
			node* _array;
		};
	public:
		inline explicit memory_pool(void) noexcept = default;
		//not implemented
		inline explicit memory_pool(memory_pool const& rhs) noexcept;
		//not implemented
		inline explicit memory_pool(memory_pool&& rhs) noexcept;
		//not implemented
		inline auto operator=(memory_pool const& rhs) noexcept -> memory_pool&;
		//not implemented
		inline auto operator=(memory_pool&& rhs) noexcept -> memory_pool&;
		inline ~memory_pool(void) noexcept {
			while (nullptr != _block) {
				block* next = _block->_next;
				_aligned_free(_block->_array);
				free(_block);
				_block = next;
			}
		}
	public:
		inline auto allocate(void) noexcept -> void* {
			if (nullptr == _head) {
				size_type capacity = static_cast<size_type>(_capacity * 1.5f);
				if (1 >= _capacity)
					capacity++;
				reserve(capacity);
			}
			void* ret = reinterpret_cast<void*>(_head);
			_head = _head->_next;
			return ret;
		}
		inline void deallocate(void* const value) noexcept {
			reinterpret_cast<node*>(value)->_next = _head;
			_head = reinterpret_cast<node*>(value);
		}
	public:
		inline void reserve(size_type capacity) noexcept {
			capacity -= _capacity;

			byte* arr = reinterpret_cast<byte*>(_aligned_malloc(_size * capacity, _align));
			reinterpret_cast<node*>(arr + _byte * (capacity - 1))->_next = _head;
			_head = reinterpret_cast<node*>(arr);

			block* cur = reinterpret_cast<block*>(malloc(sizeof(block)));
			cur->_array = reinterpret_cast<node*>(arr);
			cur->_next = _block;
			_block = cur;

			for (size_type index = 0; index < (capacity - 1); ++index, arr += _byte)
				reinterpret_cast<node*>(arr)->_next = reinterpret_cast<node*>(arr + _byte);

			_capacity += capacity;
		}
	private:
		node* _head = nullptr;
		block* _block = nullptr;
		size_type _capacity = 0;
	};
}