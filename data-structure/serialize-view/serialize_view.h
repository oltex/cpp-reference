#pragma once
#include "../serialize-buffer/serialize_buffer.h"
#include <memory>
#include <string>
#include <string_view>

namespace data_structure {
	class serialize_view final {
	private:
		using byte = unsigned char;
		using size_type = unsigned int;
		using iterator = byte*;
	public:
		inline explicit serialize_view(serialize_buffer serialize_buffer_) noexcept
			: _array(serialize_buffer_.data()), _front(serialize_buffer_.front()), _rear(serialize_buffer_.rear()), _capacity(serialize_buffer_.capacity()) {
		};
		inline explicit serialize_view(serialize_view const& rhs) noexcept
			: _array(rhs._array), _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity) {
		}
		inline explicit serialize_view(serialize_view&& rhs) noexcept
			: _array(rhs._array), _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity) {
			rhs._array = nullptr;
			rhs._front = 0;
			rhs._rear = 0;
			rhs._capacity = 0;
		}
		inline auto operator=(serialize_view const& rhs) noexcept -> serialize_view& {
			_array = rhs._array;
			_front = rhs._front;
			_rear = rhs._rear;
			_capacity = rhs._capacity;
		}
		inline auto operator=(serialize_view&& rhs) noexcept -> serialize_view& {
			_array = rhs._array;
			_front = rhs._front;
			_rear = rhs._rear;
			_capacity = rhs._capacity;
			rhs._array = nullptr;
			rhs._front = 0;
			rhs._rear = 0;
			rhs._capacity = 0;
		}
		inline ~serialize_view(void) noexcept = default;
	public:
		template<typename type>
		inline auto operator<<(type const& value) noexcept -> serialize_buffer& requires std::is_arithmetic_v<type> {
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
			if (_rear + length > _capacity) {
				_fail = true;
				return;
			}
#endif
			memcpy(_array + _rear, buffer, length);
			_rear += length;
		}
		template<string_size type>
		inline void push(std::string_view const value) noexcept {
			operator<<(static_cast<type>(value.size()));
			push((unsigned char*)value.data(), static_cast<size_type>(sizeof(std::string::value_type) * value.size()));
		}
		template<string_size type>
		inline void push(std::wstring_view const value) noexcept {
			operator<<(static_cast<type>(value.size()));
			push((unsigned char*)value.data(), static_cast<size_type>(sizeof(std::wstring::value_type) * value.size()));
		}
		template<typename type>
		inline auto operator>>(type& value) noexcept -> serialize_buffer& requires std::is_arithmetic_v<type> {
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
			if (_front + length > _rear) {
				_fail = true;
				return;
			}
#endif
			memcpy(buffer, _array + _front, length);
		}
		template<string_size type>
		inline void peek(std::string& value) noexcept {
			type length;
			peek(reinterpret_cast<byte*>(&length), sizeof(type));
#ifdef debug
			if (_front + sizeof(type) + length > _rear) {
				_fail = true;
				return;
			}
#endif
			value.assign(reinterpret_cast<char*>(_array + _front + sizeof(type)), length);
		}
		template<string_size type>
		inline void peek(std::wstring& value) noexcept {
			type length;
			peek(reinterpret_cast<byte*>(&length), sizeof(type));
#ifdef debug
			if (_front + sizeof(type) + length > _rear) {
				_fail = true;
				return;
			}
#endif
			value.assign(reinterpret_cast<wchar_t*>(_array + _front + sizeof(type)), length);
		}
		inline void pop(size_type const length) noexcept {
			_front += length;
		}
		template<string_size type>
		inline void pop(std::string const& value) noexcept {
			_front += sizeof(type) + sizeof(std::string::value_type) * value.size();
		}
		template<string_size type>
		inline void pop(std::wstring const& value) noexcept {
			_front += sizeof(type) + sizeof(std::wstring::value_type) * value.size();
		}
		inline auto begin(void) noexcept -> iterator {
			return _array + _front;
		}
		inline auto end(void) noexcept -> iterator {
			return _array + _rear;
		}
	public:
#ifdef debug
		inline operator bool(void) const noexcept {
			return !_fail;
		}
		inline auto fail(void) const noexcept {
			return _fail;
		}
#endif
	public:
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
		inline bool empty(void) const noexcept {
			return _front == _rear;
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
		inline auto data(void) noexcept -> byte* {
			return _array;
		}
	private:
		size_type _front;
		size_type _rear;
		size_type _capacity;
		byte* _array;
	};
}