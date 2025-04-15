#pragma once
#include <Windows.h>

namespace library::system::time {
	class query_performance final {
	public:
		enum class initialize : unsigned char { counter, frequency };
	public:
		inline explicit query_performance(void) noexcept = default;
		inline explicit query_performance(initialize initialize_) noexcept {
			switch (initialize_) {
			case initialize::counter:
				counter();
				break;
			case initialize::frequency:
				frequency();
				break;
			}
		};
		inline explicit query_performance(LARGE_INTEGER large_integer) noexcept
			: _large_integer(large_integer) {
		}
		inline explicit query_performance(LONGLONG quad_part) noexcept {
			_large_integer.QuadPart = quad_part;
		}
		inline explicit query_performance(query_performance const& rhs) noexcept
			: _large_integer(rhs._large_integer) {
		};
		inline explicit query_performance(query_performance&& rhs) noexcept
			: _large_integer(rhs._large_integer) {
		}
		inline auto operator=(query_performance const& rhs) noexcept -> query_performance& {
			_large_integer = rhs._large_integer;
			return *this;
		}
		inline auto operator=(query_performance&& rhs) noexcept -> query_performance& {
			_large_integer = rhs._large_integer;
			return *this;
		}
		inline ~query_performance(void) noexcept = default;
	public:
		inline void counter(void) noexcept {
			QueryPerformanceCounter(&_large_integer);
		}
		inline void frequency(void) noexcept {
			QueryPerformanceFrequency(&_large_integer);
		}
		inline void clear(void) noexcept {
			_large_integer.QuadPart = 0;
		}
		inline auto operator+(query_performance const& rhs) noexcept ->query_performance {
			return query_performance(_large_integer.QuadPart + rhs._large_integer.QuadPart);
		}
		inline auto operator+=(query_performance const& rhs) noexcept ->query_performance& {
			_large_integer.QuadPart += rhs._large_integer.QuadPart;
			return *this;
		}
		inline auto operator-(query_performance const& rhs) noexcept ->query_performance {
			return query_performance(_large_integer.QuadPart - rhs._large_integer.QuadPart);
		}
		inline auto operator-=(query_performance const& rhs) noexcept ->query_performance& {
			_large_integer.QuadPart -= rhs._large_integer.QuadPart;
			return *this;
		}
		inline auto operator/(query_performance const& rhs) noexcept ->query_performance {
			return query_performance(_large_integer.QuadPart / rhs._large_integer.QuadPart);
		}
		inline auto operator/=(query_performance const& rhs) noexcept ->query_performance& {
			_large_integer.QuadPart /= rhs._large_integer.QuadPart;
			return *this;
		}
	public:
		inline auto data(void) noexcept -> LARGE_INTEGER& {
			return _large_integer;
		}
	private:
		LARGE_INTEGER _large_integer;
	};
	inline static query_performance _frequency(query_performance::initialize::frequency);
}