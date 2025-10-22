#pragma once
#include "../memory.h"
#include "../function.h"
#include "../template.h"
#include "array.h"
#include <cassert>

namespace detail {
	template<typename type, size_t sso = 16> //small string optimization
		requires (library::any_of_type<type, char, wchar_t>)
	class string final {
		template<typename type>
		friend class string_view;
		using size_type = unsigned int;
		union buffer {
			library::array<type, sso> _array;
			type* _pointer;
		};

		size_type _size;
		size_type _capacity;
		buffer _buffer;
	public:
		using iterator = type*;

		inline explicit string(void) noexcept
			: _size(0), _capacity(sso), _buffer() {
		};
		template<typename argument>
		inline string(argument&& arg) noexcept
			: string() {
			insert(end(), std::forward<argument>(arg));
		}
		inline string(string const& rhs) noexcept
			: string(const_cast<string&>(rhs).data()) {
		};
		inline string(string&& rhs) noexcept
			: _size(library::exchange(rhs._size, 0)), _capacity(library::exchange(rhs._capacity, static_cast<size_type>(sso))), _buffer(rhs._buffer) {
			rhs.null();
		};
		inline auto operator=(string const& rhs) noexcept -> string& {
			assign(const_cast<string&>(rhs).c_str());
			return *this;
		};
		inline auto operator=(string&& rhs) noexcept -> string& {
			if (sso < _capacity)
				library::deallocate(_buffer._pointer);

			_size = library::exchange(rhs._size, 0);
			_capacity = library::exchange(rhs._capacity, static_cast<size_type>(sso));
			_buffer = rhs._buffer;
			rhs.null();
			return *this;
		};
		inline ~string(void) noexcept {
			if (sso < _capacity)
				library::deallocate(_buffer._pointer);
		};

		template<typename argument>
		//requires (library::same_type<library::remove_cp<argument>, string_view<type>> || library::same_type<library::remove_cp<argument>, type>)
		inline auto insert(iterator iter, argument&& arg) noexcept -> iterator {
			size_type char_size;
			if constexpr (library::any_of_type<library::remove_cvr<argument>, string<type>, string_view<type>>)
				char_size = arg.size();
			else if constexpr (library::same_type<library::remove_reference<argument>, type>)
				char_size = 1;
			else
				char_size = static_cast<size_type>(library::string_length(arg));

			if (_size + char_size >= _capacity) {
				auto index = iter - begin();
				reserve(library::maximum(static_cast<size_type>(_capacity * 1.5f), _size + char_size + 1));
				iter = begin() + index;
			}
			library::memory_move(iter + char_size, iter, end() - iter);

			if constexpr (library::any_of_type<library::remove_cvr<argument>, string<type>, string_view<type>>)
				library::memory_copy(iter, arg.data(), char_size);
			else if constexpr (library::same_type<library::remove_reference<argument>, type>)
				*iter = arg;
			else
				library::memory_copy(iter, arg, char_size);
			_size += char_size;
			null();
			return iter;
		}
		inline auto push_back(type character) noexcept -> type& {
			return *insert(end(), character);
		}
		inline auto erase(iterator iter) noexcept -> iterator {
			assert(_size > 0 && "called on empty");
			library::memory_move(iter, iter + 1, end() - (iter + 1));
			--_size;
			null();
			return iter;
		}
		inline void pop_back(void) noexcept {
			erase(end() - 1);
		}
		template<typename argument>
		inline auto append(argument&& arg) noexcept -> string& {
			insert(end(), std::forward<argument>(arg));
			return *this;
		}
		template<typename argument>
		inline auto operator+=(argument&& arg) noexcept -> string& {
			return append(std::forward<argument>(arg));
		}
		template<typename argument>
		inline auto assign(argument arg) noexcept -> string& {
			clear();
			return append(arg);
		}
		template<typename argument>
		inline auto operator=(argument arg) noexcept -> string& {
			return assign(arg);
		}
		template<typename argument>
		inline auto operator==(argument&& rhs) noexcept -> bool {
			size_type char_size;
			if constexpr (library::any_of_type<library::remove_cvr<argument>, string<type>, string_view<type>>)
				char_size = rhs.size();
			else if constexpr (library::same_type<library::remove_reference<argument>, type>)
				char_size = 1;
			else
				char_size = static_cast<size_type>(library::string_length(std::forward<argument>(rhs)));

			if (char_size != size())
				return false;
			if constexpr (library::any_of_type<library::remove_cvr<argument>, string<type>, string_view<type>>)
				return 0 == library::memory_compare(data(), rhs.data(), _size);
			else if constexpr (library::same_type<library::remove_reference<argument>, type>)
				return data()[0] == rhs;
			else
				return 0 == library::memory_compare(data(), std::forward<argument>(rhs), _size);
		};
		template<typename argument>
		inline auto operator+(argument&& rhs) noexcept -> string {
			string result;
			size_type char_size;
			if constexpr (library::any_of_type<library::remove_cvr<argument>, string<type>, string_view<type>>)
				char_size = rhs.size();
			else if constexpr (library::same_type<library::remove_reference<argument>, type>)
				char_size = 1;
			else
				char_size = static_cast<size_type>(library::string_length(std::forward<argument>(rhs)));

			result.reserve(size() + char_size + 1);
			result.append(*this).append(std::forward<argument>(rhs));
			return result;
		}
		template<typename argument>
		inline friend auto operator+(argument&& lhs, string const& rhs) noexcept -> string {
			string result;
			size_type char_size;
			if constexpr (library::any_of_type<library::remove_cvr<argument>, string<type>, string_view<type>>)
				char_size = lhs.size();
			else if constexpr (library::same_type<library::remove_reference<argument>, type>)
				char_size = 1;
			else
				char_size = static_cast<size_type>(library::string_length(std::forward<argument>(lhs)));

			result.reserve(char_size + rhs.size() + 1);
			result.append(std::forward<argument>(lhs)).append(rhs);
			return result;
		}

		inline auto begin(void) noexcept -> iterator {
			return data();
		}
		inline auto end(void) noexcept -> iterator {
			return data() + _size;
		}
		inline auto front(void) const noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return data()[0];
		}
		inline auto back(void) const noexcept -> type& {
			assert(_size > 0 && "called on empty");
			return data()[_size - 1];
		}
		inline auto operator[](size_type const index) const noexcept ->type& {
			assert(index < _size && "index out of range");
			return data()[index];
		}
		inline void reserve(size_type capacity) noexcept {
			if (_capacity < capacity) {
				if (sso >= _capacity)
					_buffer._pointer = reinterpret_cast<type*>(library::memory_copy(library::allocate<type>(capacity), _buffer._array.data(), _size + 1));
				else
					_buffer._pointer = reinterpret_cast<type*>(library::reallocate(_buffer._pointer, capacity));
				_capacity = capacity;
			}
		}
		inline void clear(void) noexcept {
			_size = 0;
			null();
		}
		inline void swap(string& rhs) noexcept {
			library::swap(_size, rhs._size);
			library::swap(_capacity, rhs._capacity);
			library::swap(_buffer, rhs._buffer);
		}
		inline bool empty(void) const noexcept {
			return 0 == _size;
		}
		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto data(void) noexcept -> type* {
			return const_cast<type*>(static_cast<string const&>(*this).data());
		}
		inline auto data(void) const noexcept -> type const* {
			if (sso >= _capacity)
				return _buffer._array.data();
			else
				return _buffer._pointer;
		}
	private:
		inline void null(void) noexcept {
			if constexpr (library::same_type<type, char>)
				data()[_size] = '\0';
			else
				data()[_size] = L'\0';
		}
	};

	template<typename type>
	class string_view {
		using size_type = unsigned int;
		using iterator = type*;
		type const* _pointer;
		size_type _size;
	public:
		inline explicit string_view(void) noexcept
			: _pointer(nullptr), _size(0) {
		};
		inline explicit string_view(string<type> const& string) noexcept
			: _pointer(string.data()), _size(string.size()) {
		};
		inline explicit string_view(type const* const pointer) noexcept
			: _pointer(pointer), _size(static_cast<size_type>(library::string_length(pointer))) {
		}
		inline string_view(string_view const&) noexcept = default;
		inline string_view(string_view&&) noexcept = default;
		inline auto operator=(string_view const&) noexcept -> string_view & = default;
		inline auto operator=(string_view&&) noexcept -> string_view & = default;
		inline ~string_view(void) noexcept = default;

		inline auto size(void) const noexcept -> size_type {
			return _size;
		}
		inline auto begin(void) noexcept -> iterator {
			return _pointer;
		}
		inline auto end(void) noexcept -> iterator {
			return _pointer + _size;
		}
		inline auto operator[](size_type const index) const noexcept ->type& {
			assert(index < _size && "index out of range");
			return _pointer[index];
		}
		inline auto data(void) noexcept -> type const* {
			return _pointer;
		}
	};

	template<typename type, size_t size>
	struct string_literal {
		type _value[size];
		inline constexpr string_literal(const type(&str)[size]) noexcept {
			std::copy_n(str, size, _value);
		}
		inline constexpr explicit string_literal(string_literal const&) noexcept = default;
		inline constexpr explicit string_literal(string_literal&&) noexcept = default;
		inline auto operator=(string_literal const&) noexcept -> string_literal & = default;
		inline auto operator=(string_literal&&) noexcept -> string_literal & = default;
		inline ~string_literal(void) noexcept = default;
	};
}

namespace library {
	using string = typename detail::string<char>;
	using wstring = typename detail::string<wchar_t>;
	using string_view = typename detail::string_view<char>;
	using wstring_view = typename detail::string_view<wchar_t>;
	template<size_t size>
	using string_literal = typename detail::string_literal<char, size>;
	template<size_t size>
	using wstring_literal = typename detail::string_literal<wchar_t, size>;

	template<typename type>
		requires (library::any_of_type<type, string, wstring>)
	struct fnv_hash_string {
		using size_type = unsigned int;
		inline static constexpr size_type _offset_basis = sizeof(size_type) == 4 ? 2166136261U : 14695981039346656037ULL;
		inline static constexpr size_type _prime = sizeof(size_type) == 4 ? 16777619U : 1099511628211ULL;

		inline static constexpr auto execute(type const& key) -> size_type {
			auto value = _offset_basis;
			auto byte = reinterpret_cast<unsigned char const*>(key.data());
			for (size_type index = 0; index < key.size(); ++index) {
				value ^= static_cast<size_type>(byte[index]);
				value *= _prime;
			}
			return value;
		}
		template <typename argument>
			requires (library::same_type<type, string>&& library::same_type<argument, char>) || (library::same_type<type, wstring> && library::same_type<argument, wchar_t>)
		inline static constexpr auto execute(argument const* key) -> size_type {
			auto value = _offset_basis;
			auto byte = reinterpret_cast<unsigned char const*>(key);
			auto size = library::string_length(key);
			for (size_type index = 0; index < size; ++index) {
				value ^= static_cast<size_type>(byte[index]);
				value *= _prime;
			}
			return value;
		}
	};
	template<>
	struct fnv_hash<string> : public fnv_hash_string<string> {
	};
	template<>
	struct fnv_hash<wstring> : public fnv_hash_string<wstring> {
	};
}