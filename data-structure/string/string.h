#pragma once

namespace library {
	template<auto sso = 16> //small string optimization
	class string final {
		using size_type = unsigned int;
	public:
		inline explicit string(void) noexcept = default;
		inline string(string const& rhs) noexcept = default;
		inline explicit string(string&& rhs) noexcept = default;
		inline auto operator=(string const& rhs) noexcept -> string & = default;
		inline auto operator=(string&& rhs) noexcept -> string & = default;
		inline ~string(void) noexcept = default;

		size_type _size;
		size_type _capacity;
		union {

		} _buffer;
	};
}