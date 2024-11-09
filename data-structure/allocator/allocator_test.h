#pragma once
#include <memory>

namespace data_structure {
	template<typename type>
	class allocator_test final {
		using size_type = unsigned int;
	private:
		class memory final {
		private:
			struct node final {
				node* _next;
			};
			struct pool final {
				node* _array;
				pool* _next;
			};
		public:
			inline explicit memory(size_type const byte) noexcept
				: _byte(byte) {
			};
			inline ~memory(void) noexcept {
				while (nullptr != _pool) {
#pragma warning(suppress: 6001)
					pool* next = _pool->_next;
					_aligned_free(_pool->_array);
					free(_pool);
					_pool = next;
				}
			}
		public:
			inline auto allocate(void) noexcept -> type* {
				if (nullptr == _vacant) {
					_capacity = static_cast<size_type>(_capacity * 1.5f);
					if (1 >= _capacity)
						_capacity++;
					reserve(_capacity);
				}
				type* res = reinterpret_cast<type*>(_vacant);
				_vacant = _vacant->_next;
				return res;
			}
			inline void deallocate(type* const value) noexcept {
				reinterpret_cast<node*>(value)->_next = _vacant;
				_vacant = reinterpret_cast<node*>(value);
			}
			inline void reserve(size_type const capacity) noexcept {
				_capacity = capacity;

				size_type size = _byte * _capacity;
				unsigned char* arr = static_cast<unsigned char*>(_aligned_malloc(size, alignof(type) < alignof(node) ? alignof(node) : alignof(type)));
				//#pragma warning(suppress: 6387)
				//			memset(arr, 0, size);

				size = size / _byte - 1;
#pragma warning(suppress: 6011)
				reinterpret_cast<node*>(arr + size * _byte)->_next = _vacant;
				_vacant = reinterpret_cast<node*>(arr);

				pool* head = static_cast<pool*>(malloc(sizeof(pool)));
#pragma warning(suppress: 6011)
				head->_array = reinterpret_cast<node*>(arr);
				head->_next = _pool;
				_pool = head;

				for (size_type idx = 0; idx < size; ++idx, arr += _byte)
#pragma warning(suppress: 6011)
					reinterpret_cast<node*>(arr)->_next = reinterpret_cast<node*>(arr + _byte);
			}
		private:
			node* _vacant = nullptr;
			pool* _pool = nullptr;
			size_type _capacity = 0;
			size_type const _byte;
		};
	public:
		inline explicit allocator_test(void) noexcept {
			//for (size_type idx = 8; idx < 4096; idx *= 2)
			memset(_memory, 0, sizeof(memory*) * 10);

		};
		inline ~allocator_test(void) noexcept {
			for (auto idx = 0; idx < 10; ++idx)
				if (nullptr != _memory[idx])
					delete _memory[idx];
		}
	public:
		inline auto allocate(size_type const size) noexcept -> type* {
			size_type byte = sizeof(type) * size;
			if (byte > 4096)
				return static_cast<type*>(malloc(byte));

			unsigned long idx;
			_BitScanReverse64(&idx, byte - 1);
			idx -= 2;

			if (nullptr == _memory[idx])
				_memory[idx] = new memory(static_cast<size_type>(pow(2, idx + 3)));
			return _memory[idx]->allocate();
		}
		inline void deallocate(type* const value, size_type const size) noexcept {
			size_type byte = sizeof(type) * size;
			if (byte > 4096) {
				free(value);
				return;
			}

			unsigned long idx;
			_BitScanReverse64(&idx, byte - 1);
			idx -= 2;

			_memory[idx]->deallocate(value);
		}
	private:
		memory* _memory[10];
	};
}