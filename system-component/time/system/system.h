#pragma once
#include <Windows.h>

namespace system_component::time {
	class system_32 final {
	public:
		inline explicit system_32(void) noexcept = default;
		inline explicit system_32(system_32 const& rhs) noexcept
			: _tick_count(rhs._tick_count) {
		};
		inline explicit system_32(system_32&& rhs) noexcept
			: _tick_count(rhs._tick_count) {
		}
		inline auto operator=(system_32 const& rhs) noexcept -> system_32& {
			_tick_count = rhs._tick_count;
		}
		inline auto operator=(system_32&& rhs) noexcept -> system_32& {
			_tick_count = rhs._tick_count;
		}
		inline ~system_32(void) noexcept = default;
	public:
		inline auto get_tick_count(void) noexcept -> unsigned long {
#pragma warning(suppress: 28159)
			return _tick_count = GetTickCount();
		}
	public:
		inline auto data(void) noexcept -> unsigned long& {
			return _tick_count;
		}
	private:
		unsigned long _tick_count;
	};
	class system_64 final {
	public:
		inline explicit system_64(void) noexcept = default;
		inline explicit system_64(system_64 const& rhs) noexcept
			: _tick_count(rhs._tick_count) {
		};
		inline explicit system_64(system_64&& rhs) noexcept
			: _tick_count(rhs._tick_count) {
		}
		inline auto operator=(system_64 const& rhs) noexcept -> system_64& {
			_tick_count = rhs._tick_count;
		}
		inline auto operator=(system_64&& rhs) noexcept -> system_64& {
			_tick_count = rhs._tick_count;
		}
		inline ~system_64(void) noexcept = default;
	public:
		inline auto get_tick_count(void) noexcept -> unsigned long long {
			return _tick_count = GetTickCount64();
		}
	private:
		unsigned long long _tick_count;
	};
}