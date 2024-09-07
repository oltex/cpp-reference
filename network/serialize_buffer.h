#pragma once
#include <memory>
#include <string>

namespace data_structure {
	class serialize_buffer final {
		using byte = unsigned char;
		using size_type = unsigned int;
	public:
		inline explicit serialize_buffer(void) noexcept {
			reserve(1024);
		};
		inline explicit serialize_buffer(serialize_buffer const& rhs) noexcept
			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity), _fail(rhs._fail) {
			_array = new byte(_capacity);
			memcpy(_array, rhs._array, _rear);
		}
		inline auto operator=(serialize_buffer const& rhs) noexcept -> serialize_buffer& {
			_front = rhs._front;
			_rear = rhs._rear;
			_capacity = rhs._capacity;
			_fail = rhs._fail;
			_array = new byte(_capacity);
			memcpy(_array, rhs._array, _rear);
		}
		inline explicit serialize_buffer(serialize_buffer&& rhs) noexcept
			: _front(rhs._front), _rear(rhs._rear), _capacity(rhs._capacity), _fail(rhs._fail), _array(rhs._array) {
			rhs._array = nullptr;
		}
		inline auto operator=(serialize_buffer&& rhs) noexcept -> serialize_buffer& {
			_front = rhs._front;
			_rear = rhs._rear;
			_capacity = rhs._capacity;
			_fail = rhs._fail;
			_array = rhs._array;
			rhs._array = nullptr;
		}
		inline ~serialize_buffer(void) noexcept {
			free(_array);
		};
	public:
		template<typename type>
		inline auto operator<<(type const& value) noexcept -> serialize_buffer& requires std::is_arithmetic_v<type> {
			if (sizeof(type) + _rear > _capacity) {
				_fail = true;
				return *this;
			}
			reinterpret_cast<type&>(_array[_rear]) = value;
			_rear += sizeof(type);
			return *this;
		}
		inline void push(byte* const buffer, size_type const length) noexcept {
			if (_rear + length > _capacity) {
				_fail = true;
				return;
			}
			memcpy(_array + _rear, buffer, length);
			_rear += length;
		}
		template<typename string_size_type>
		inline void push(std::string const& value) noexcept requires std::_Is_any_of_v<string_size_type, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long> {
			operator<<(static_cast<string_size_type>(value.size()));
			push((unsigned char*)value.c_str(), static_cast<size_type>(sizeof(std::string::value_type) * value.size()));
		}
		template<typename string_size_type>
		inline void push(std::wstring const& value) noexcept requires std::_Is_any_of_v<string_size_type, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long> {
			operator<<(static_cast<string_size_type>(value.size()));
			push((unsigned char*)value.c_str(), static_cast<size_type>(sizeof(std::wstring::value_type) * value.size()));
		}
		template<typename type>
		inline auto operator>>(type& value) noexcept -> serialize_buffer& requires std::is_arithmetic_v<type> {
			if (sizeof(type) + _front > _rear) {
				_fail = true;
				return *this;
			}
			value = reinterpret_cast<type&>(_array[_front]);
			_front += sizeof(type);
			return *this;
		}
		inline void pop(byte* const buffer, size_type const length) noexcept {
			if (_front + length > _rear) {
				_fail = true;
				return;
			}
			memcpy(_array + _front, buffer, length);
			_front += length;
		}
		template<typename string_size_type>
		inline void pop(std::string& value) noexcept requires std::_Is_any_of_v<string_size_type, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long> {
			string_size_type length;
			operator>>(length);
			if (_front + length > _rear) {
				_fail = true;
				return;
			}
			value.assign(reinterpret_cast<char*>(_array + _front), length);
			_front += length;
		}
		template<typename string_size_type>
		inline void pop(std::wstring& value) noexcept requires std::_Is_any_of_v<string_size_type, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long> {
			string_size_type length;
			operator>>(length);
			if (_front + length > _rear) {
				_fail = true;
				return;
			}
			value.assign(reinterpret_cast<wchar_t*>(_array + _front), length);
			_front += length;
		}
	public:
		inline auto data(void) const noexcept -> byte* {
			return _array;
		}
	public:
		inline operator bool(void) const noexcept {
			return !_fail;
		}
		inline auto fail(void) const noexcept {
			return _fail;
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
		inline void reserve(size_type const& capacity) noexcept {
			if (_rear > capacity)
				return;
			_capacity = capacity;
#pragma warning(suppress: 6308)
			_array = static_cast<byte*>(realloc(_array, _capacity));
		}
		inline void clear(void) noexcept {
			_front = _rear = 0;
			_fail = false;
		}
	public:
		inline void move_front(size_type const length) noexcept {
			_front += length;
		}
		inline void move_rear(size_type const length) noexcept {
			_rear += length;
		}
	private:
		size_type _front = 0;
		size_type _rear = 0;
		size_type _capacity = 0;
		bool _fail = false;
		byte* _array = nullptr;
	};
}