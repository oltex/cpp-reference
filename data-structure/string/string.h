#pragma once
#include <cassert>
#include "../../memory/memory.h"
#include "../../function/function.h"

namespace library {
	template<auto sso = 3> //small string optimization
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
		using iterator = char*;

		inline explicit string(void) noexcept
			: _size(0), _capacity(sso), _buffer() {
		};
		inline string(string const& rhs) noexcept = default;
		inline explicit string(string&& rhs) noexcept = default;
		inline auto operator=(string const& rhs) noexcept -> string & = default;
		inline auto operator=(string&& rhs) noexcept -> string & = default;
		inline ~string(void) noexcept = default;

		template<typename type>
			requires (std::is_same_v<type, char> || std::is_same_v<type, char const*>)
		inline auto insert(iterator iter, type character) noexcept -> iterator {
			size_type char_size;
			if constexpr (std::is_same_v<type, char>)
				char_size = 1;
			else
				char_size = static_cast<size_type>(library::string_length(character));

			if (_size + char_size >= _capacity) {
				auto index = iter - begin();
				reserve(library::maximum(static_cast<size_type>(_capacity * 1.5f), _capacity + char_size + 1));
				iter = begin() + index;
			}
			library::memory_move(iter + char_size, iter, end() - iter);

			if constexpr (std::is_same_v<type, char>)
				*iter = character;
			else
				library::memory_copy(iter, character, char_size);
			_size += char_size;
			return iter;
		}
		inline auto push_back(char character) noexcept -> char& {
			return *insert(end(), character);
		}
		inline auto erase(iterator iter) noexcept -> iterator {
			assert(_size > 0 && "called on empty");
			library::memory_move(iter, iter + 1, end() - (iter + 1));
			--_size;
			return iter;
		}
		inline void pop_back(void) noexcept {
			erase(end() - 1);
		}
		inline auto operator=(char const* const character) noexcept {
			assign(character);
			return *this;
		}
		inline auto assign(char const* const character) noexcept -> string& {
			clear();
			append(character);
			return *this;
		}
		inline auto operator+=(char const* const character) noexcept -> string& {
			append(character);
			return *this;
		}
		inline auto append(char const* const character) noexcept -> string& {
			insert(end(), character);
			return *this;
		}

		inline auto begin(void) noexcept -> iterator {
			return data();
		}
		inline auto end(void) noexcept -> iterator {
			return data() + _size;
		}
		inline auto front(void) const noexcept -> char& {
			assert(_size > 0 && "called on empty");
			return data()[0];
		}
		inline auto back(void) const noexcept -> char& {
			assert(_size > 0 && "called on empty");
			return data()[_size - 1];
		}
		inline auto operator[](size_type const index) const noexcept ->char& {
			assert(index < _size && "index out of range");
			return data()[index];
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