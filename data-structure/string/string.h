#pragma once
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<auto sso = 16> //small string optimization
	class string final {
		using size_type = unsigned int;
		union buffer {
			char _buffer[sso];
			char* _pointer;
		};
		size_type _size;
		size_type _capacity;
		buffer _buffer;
	public:
		inline explicit string(void) noexcept
			: _size(0), _capacity(sso), _buffer() {
		};
		inline string(string const& rhs) noexcept = default;
		inline explicit string(string&& rhs) noexcept = default;
		inline auto operator=(string const& rhs) noexcept -> string & = default;
		inline auto operator=(string&& rhs) noexcept -> string & = default;
		inline ~string(void) noexcept = default;


		inline auto push_back(char character) noexcept -> char& {
			if (_size + 1 >= _capacity)
				reserve(maximum(static_cast<size_type>(_capacity * 1.5f), _capacity + 1));
			return data()[_size++] = character;
		}

		inline auto operator=(char const* const character) noexcept {
			//if (_size >= _capacity)
			//	reserve(library::maximum(static_cast<size_type>(_capacity * 1.5f), _size + 1));
			return *this;
		}
		inline auto operator+=(char const* const character) noexcept {
			if (_size >= _capacity)
				reserve(library::maximum(static_cast<size_type>(_capacity * 1.5f), _size + 1));


			return *this;
		}




		inline void reserve(size_type capacity) noexcept {
			if (_capacity < capacity) {
				if (sso >= _capacity)
					_buffer._pointer = reinterpret_cast<char*>(library::memory_copy(library::allocate(capacity), _buffer._buffer, _size));
				else
					_buffer._pointer = reinterpret_cast<char*>(library::reallocate(_buffer._pointer, capacity));
				_capacity = capacity;
			}
		}
		inline void clear(void) noexcept {
			_size = 0;
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto data(void) noexcept -> char* {
			if (sso >= _capacity)
				return _buffer._buffer;
			else
				return _buffer._pointer;
		}
		//inline auto large_mode_engaged(void) noexcept -> bool {
		//	return true;
		//}
	};
}