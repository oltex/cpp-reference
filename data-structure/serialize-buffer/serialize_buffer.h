#pragma once
//#define debug
#include <memory>
#include <string>
#include <string_view>
#include "../../system/memory/memory.h"

namespace library::data_structure {
	class serialize_buffer final {
	private:
		using byte = unsigned char;
		using size_type = unsigned int;
		using iterator = byte*;
	public:
		inline explicit serialize_buffer(void) noexcept {
			reserve(1024);
		};
		inline explicit serialize_buffer(serialize_buffer const& rhs) noexcept
			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity)
#ifdef debug
			, _fail(rhs._fail)
#endif
		{
			_array = reinterpret_cast<byte*>(malloc(_capacity));
#pragma warning(suppress: 6387)
			memcpy(_array, rhs._array, _rear);
		}
		inline explicit serialize_buffer(serialize_buffer&& rhs) noexcept
			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity),
#ifdef debug
			_fail(rhs._fail),
#endif
			_array(rhs._array) {
			rhs._array = nullptr;
		}
		inline auto operator=(serialize_buffer const& rhs) noexcept -> serialize_buffer&;
		inline auto operator=(serialize_buffer&& rhs) noexcept -> serialize_buffer&;
		inline ~serialize_buffer(void) noexcept {
			system::memory::deallocate<byte>(_array);
		};

		template<typename type>
			requires std::is_arithmetic_v<type>
		inline auto operator<<(type const value) noexcept -> serialize_buffer&  {
#ifdef debug
			if (sizeof(type) + _rear > _capacity) {
				_fail = true;
				return *this;
			}
#endif
			reinterpret_cast<type&>(_array[_rear]) = value;
			_rear += sizeof(type);
			return *this;
		}
		inline void push(byte* const buffer, size_type const length) noexcept {
#ifdef debug
			if (length + _rear > _capacity) {
				_fail = true;
				return;
			}
#endif
			memcpy(_array + _rear, buffer, length);
			_rear += length;
		}
		template<typename type>
			requires std::is_arithmetic_v<type>
		inline auto operator>>(type& value) noexcept -> serialize_buffer&  {
#ifdef debug
			if (sizeof(type) + _front > _rear) {
				_fail = true;
				return *this;
			}
#endif
			value = reinterpret_cast<type&>(_array[_front]);
			_front += sizeof(type);
			return *this;
		}
		inline void peek(byte* const buffer, size_type const length) const noexcept {
#ifdef debug
			if (length + _front > _rear) {
				_fail = true;
				return;
			}
#endif
			memcpy(buffer, _array + _front, length);
		}
		inline void pop(size_type const length) noexcept {
			_front += length;
		}
		inline auto begin(void) noexcept -> iterator {
			return _array + _front;
		}
		inline auto end(void) noexcept -> iterator {
			return _array + _rear;
		}
#ifdef debug
		inline operator bool(void) const noexcept {
			return !_fail;
		}
		inline auto fail(void) const noexcept {
			return _fail;
		}
#endif
		inline void reserve(size_type const& capacity) noexcept {
			if (_rear <= capacity) {
				_capacity = capacity;
#pragma warning(suppress: 6308)
				_array = reinterpret_cast<byte*>(realloc(_array, _capacity));
			}
		}
		inline void clear(void) noexcept {
			_front = _rear = 0;
#ifdef debug
			_fail = false;
#endif
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
		inline auto capacity(void) const noexcept -> size_type {
			return _capacity;
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
	private:
		size_type _front = 0;
		size_type _rear = 0;
		size_type _capacity = 0;
#ifdef debug
		bool _fail = false;
#endif
		byte* _array = nullptr;
	};
}

//template<typename... argument>
//inline void test(argument&&... arg) noexcept {
//	((reinterpret_cast<argument&>(_array[_rear]) = std::forward<argument>(arg), _rear += sizeof(argument)), ...);
//}

//template<typename type>
//inline auto push(std::string const& value) noexcept -> size_type {
//	operator<<(static_cast<type>(value.size()));
//	return push((unsigned char*)value.c_str(), int(sizeof(std::string::value_type) * value.size()));
//}