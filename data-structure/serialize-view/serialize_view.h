#pragma once
#include <memory>
#include <string>
#include <string_view>

namespace data_structure {
	template<typename type>
	concept string_size = std::_Is_any_of_v<type, unsigned char, unsigned short, unsigned int, unsigned long, unsigned long long>;

	class serialize_view final {
	private:
		using byte = unsigned char;
		using size_type = unsigned int;
	public:
		inline explicit serialize_view(byte* array_, size_type current) noexcept
			: _array(array_), _current(current) {
		};
		inline explicit serialize_view(serialize_view const& rhs) noexcept
			: _array(rhs._array), _current(rhs._current) {
		}
		inline explicit serialize_view(serialize_view&& rhs) noexcept
			: _array(rhs._array), _current(rhs._current) {
			rhs._array = nullptr;
			rhs._current = 0;
		}
		inline auto operator=(serialize_view const& rhs) noexcept -> serialize_view& {
			_array = rhs._array;
			_current = rhs._current;
		}
		inline auto operator=(serialize_view&& rhs) noexcept -> serialize_view& {
			_array = rhs._array;
			_current = rhs._current;
			rhs._array = nullptr;
			rhs._current = 0;
		}
		inline ~serialize_view(void) noexcept = default;
	public:
		template<typename type>
		inline auto operator>>(type& value) noexcept -> serialize_view& requires std::is_arithmetic_v<type> {
			value = reinterpret_cast<type&>(_array[_front]);
			_front += sizeof(type);
			return *this;
		}
		inline void peek(byte* const buffer, size_type const length) noexcept {
			memcpy(buffer, _array + _current, length);
		}
		template<string_size type>
		inline void peek(std::string& value) noexcept {
			type length;
			peek(reinterpret_cast<byte*>(&length), sizeof(type));
			value.assign(reinterpret_cast<char*>(_array + _current + sizeof(type)), length);
		}
		template<string_size type>
		inline void peek(std::wstring& value) noexcept {
			type length;
			peek(reinterpret_cast<byte*>(&length), sizeof(type));
			value.assign(reinterpret_cast<wchar_t*>(_array + _current + sizeof(type)), length);
		}
		inline void pop(size_type const length) noexcept {
			_current += length;
		}
		template<string_size type>
		inline void pop(std::string const& value) noexcept {
			_current += sizeof(type) + sizeof(std::string::value_type) * value.size();
		}
		template<string_size type>
		inline void pop(std::wstring const& value) noexcept {
			_current += sizeof(type) + sizeof(std::wstring::value_type) * value.size();
		}
	public:
		inline auto data(void) noexcept -> byte* {
			return _array;
		}
	private:
		size_type _current;
		byte* _array;
	};
}