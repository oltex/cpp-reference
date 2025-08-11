#pragma once
#include <memory>
#include <cassert>
#include "../../memory/memory.h"
#include "../../function/function.h"
#include "../../template/template.h"

namespace detail {
	template <size_t capacity_ = 128>
	class serialize_buffer {
	protected:
		using byte = unsigned char;
		using size_type = unsigned int;
		size_type _front;
		size_type _rear;
		byte _array[capacity_];
	public:
		inline serialize_buffer(void) noexcept
			: _front(0), _rear(0) {
		};
		inline explicit serialize_buffer(serialize_buffer const&) noexcept = default;
		inline explicit serialize_buffer(serialize_buffer&&) noexcept = default;
		inline auto operator=(serialize_buffer const&) noexcept -> serialize_buffer & = default;
		inline auto operator=(serialize_buffer&&) noexcept -> serialize_buffer & = default;
		inline ~serialize_buffer(void) noexcept = default;

		template<typename type>
			requires library::arithmetic_type<type>
		inline auto operator<<(type const value) noexcept -> serialize_buffer& {
			assert(sizeof(type) + _rear <= capacity_ && "not enough capacity");
			reinterpret_cast<type&>(_array[_rear]) = value;
			_rear += sizeof(type);
			return *this;
		}
		inline void push(byte* const buffer, size_type const length) noexcept {
			assert(length + _rear <= capacity_ && "not enough capacity");
			library::memory_copy(_array + _rear, buffer, length);
			_rear += length;
		}
		inline auto capacity(void) const noexcept -> size_type {
			return capacity_;
		}
	};
	template<>
	class serialize_buffer<0> {
	protected:
		using size_type = unsigned int;
		size_type _front;
		size_type _rear;
		size_type _capacity;
		byte* _array;
	public:
		inline explicit serialize_buffer(void) noexcept
			: _front(0), _rear(0), _capacity(0), _array(nullptr) {
		};
		inline explicit serialize_buffer(serialize_buffer const& rhs) noexcept
			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity), _array(reinterpret_cast<byte*>(library::allocate(_capacity))) {
			library::memory_copy(_array + _front, rhs._array + _front, _rear - _front);
		}
		inline explicit serialize_buffer(serialize_buffer&& rhs) noexcept
			: _front(library::exchange(rhs._front, 0)), _rear(library::exchange(rhs._rear, 0)), _capacity(library::exchange(rhs._capacity, 0)), _array(library::exchange(rhs._array, nullptr)) {
		}
		inline auto operator=(serialize_buffer const& rhs) noexcept -> serialize_buffer&;
		inline auto operator=(serialize_buffer&& rhs) noexcept -> serialize_buffer&;
		inline ~serialize_buffer(void) noexcept {
			library::deallocate<byte>(_array);
		};

		template<typename type>
			requires library::arithmetic_type<type>
		inline auto operator<<(type const value) noexcept -> serialize_buffer& {
			//if (sizeof(type) + _rear > _capacity) {
			//	reserve(library::maximum(static_cast<size_type>(_capacity * 1.5f), _size + 1));
			//}
			reinterpret_cast<type&>(_array[_rear]) = value;
			_rear += sizeof(type);
			return *this;
		}
		inline void push(byte* const buffer, size_type const length) noexcept {
			library::memory_copy(_array + _rear, buffer, length);
			_rear += length;
		}
		inline void reserve(size_type const& capacity) noexcept {
			if (_capacity < capacity) {
#pragma warning(suppress: 6308)
				_array = reinterpret_cast<byte*>(realloc(_array, capacity));
				_capacity = capacity;
			}
		}
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
		}
	};
}

namespace library {
	template<size_t capacity_ = 128>
	class serialize_buffer : public detail::serialize_buffer<capacity_> {
		using size_type = detail::serialize_buffer<capacity_>::size_type;
		using iterator = byte*;
		using detail::serialize_buffer<capacity_>::_front;
		using detail::serialize_buffer<capacity_>::_rear;
		using detail::serialize_buffer<capacity_>::_array;
	public:
		template<typename type>
			requires library::arithmetic_type<type>
		inline auto operator>>(type& value) noexcept -> serialize_buffer& {
			assert(sizeof(type) + _front <= _rear && "not enough data");
			value = reinterpret_cast<type&>(_array[_front]);
			_front += sizeof(type);
			return *this;
		}
		inline void peek(byte* const buffer, size_type const length) const noexcept {
			assert(length + _front <= _rear && "not enough data");
			library::memory_copy(buffer, _array + _front, length);
		}
		inline void pop(size_type const length) noexcept {
			assert(length + _front <= _rear && "not enough data");
			_front += length;
		}

		inline auto begin(void) noexcept -> iterator {
			return _array + _front;
		}
		inline auto end(void) noexcept -> iterator {
			return _array + _rear;
		}

		inline void clear(void) noexcept {
			_front = _rear = 0;
		}
		inline auto size(void) const noexcept -> size_type {
			return _rear - _front;
		}
		inline auto front(void) const noexcept -> size_type {
			return _front;
		}
		inline auto rear(void) const noexcept -> size_type {
			return _rear;
		}
		inline void move_front(size_type const length) noexcept {
			_front += length;
		}
		inline void move_rear(size_type const length) noexcept {
			_rear += length;
		}
		inline auto data(void) const noexcept -> byte* {
			return _array;
		}
	};
}

//namespace library {
//	template<size_t _capacity = 0>
//	class serialize_buffer final {
//	private:
//		using byte = unsigned char;
//		using size_type = unsigned int;
//		using iterator = byte*;
//	public:
//		inline explicit serialize_buffer(void) noexcept {
//			reserve(1024);
//		};
//		inline explicit serialize_buffer(serialize_buffer const& rhs) noexcept
//			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity)
//#ifdef debug
//			, _fail(rhs._fail)
//#endif
//		{
//			_array = reinterpret_cast<byte*>(malloc(_capacity));
//#pragma warning(suppress: 6387)
//			memcpy(_array, rhs._array, _rear);
//		}
//		inline explicit serialize_buffer(serialize_buffer&& rhs) noexcept
//			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity),
//#ifdef debug
//			_fail(rhs._fail),
//#endif
//			_array(rhs._array) {
//			rhs._array = nullptr;
//		}
//		inline auto operator=(serialize_buffer const& rhs) noexcept -> serialize_buffer&;
//		inline auto operator=(serialize_buffer&& rhs) noexcept -> serialize_buffer&;
//		inline ~serialize_buffer(void) noexcept {
//			library::deallocate<byte>(_array);
//		};
//
//		template<typename type>
//			requires std::is_arithmetic_v<type>
//		inline auto operator<<(type const value) noexcept -> serialize_buffer&  {
//#ifdef debug
//			if (sizeof(type) + _rear > _capacity) {
//				_fail = true;
//				return *this;
//			}
//#endif
//			reinterpret_cast<type&>(_array[_rear]) = value;
//			_rear += sizeof(type);
//			return *this;
//		}
//		inline void push(byte* const buffer, size_type const length) noexcept {
//#ifdef debug
//			if (length + _rear > _capacity) {
//				_fail = true;
//				return;
//			}
//#endif
//			memcpy(_array + _rear, buffer, length);
//			_rear += length;
//		}
//		template<typename type>
//			requires std::is_arithmetic_v<type>
//		inline auto operator>>(type& value) noexcept -> serialize_buffer&  {
//#ifdef debug
//			if (sizeof(type) + _front > _rear) {
//				_fail = true;
//				return *this;
//			}
//#endif
//			value = reinterpret_cast<type&>(_array[_front]);
//			_front += sizeof(type);
//			return *this;
//		}
//		inline void peek(byte* const buffer, size_type const length) const noexcept {
//#ifdef debug
//			if (length + _front > _rear) {
//				_fail = true;
//				return;
//			}
//#endif
//			memcpy(buffer, _array + _front, length);
//		}
//		inline void pop(size_type const length) noexcept {
//			_front += length;
//		}
//		inline auto begin(void) noexcept -> iterator {
//			return _array + _front;
//		}
//		inline auto end(void) noexcept -> iterator {
//			return _array + _rear;
//		}
//#ifdef debug
//		inline operator bool(void) const noexcept {
//			return !_fail;
//		}
//		inline auto fail(void) const noexcept {
//			return _fail;
//		}
//#endif
//		inline void reserve(size_type const& capacity) noexcept {
//			if (_rear <= capacity) {
//				_capacity = capacity;
//#pragma warning(suppress: 6308)
//				_array = reinterpret_cast<byte*>(realloc(_array, _capacity));
//			}
//		}
//		inline void clear(void) noexcept {
//			_front = _rear = 0;
//#ifdef debug
//			_fail = false;
//#endif
//		}
//		inline auto size(void) const noexcept -> size_type {
//			return _rear - _front;
//		}
//		inline auto front(void) const noexcept -> size_type {
//			return _front;
//		}
//		inline auto rear(void) const noexcept -> size_type {
//			return _rear;
//		}
//		inline auto capacity(void) const noexcept -> size_type {
//			return _capacity;
//		}
//		inline void move_front(size_type const length) noexcept {
//			_front += length;
//		}
//		inline void move_rear(size_type const length) noexcept {
//			_rear += length;
//		}
//		inline auto data(void) const noexcept -> byte* {
//			return _array;
//		}
//	private:
//		size_type _front = 0;
//		size_type _rear = 0;
//		size_type _capacity = 0;
//#ifdef debug
//		bool _fail = false;
//#endif
//		byte* _array = nullptr;
//	};
//}

//template<typename... argument>
//inline void test(argument&&... arg) noexcept {
//	((reinterpret_cast<argument&>(_array[_rear]) = std::forward<argument>(arg), _rear += sizeof(argument)), ...);
//}

//template<typename type>
//inline auto push(std::string const& value) noexcept -> size_type {
//	operator<<(static_cast<type>(value.size()));
//	return push((unsigned char*)value.c_str(), int(sizeof(std::string::value_type) * value.size()));
//}