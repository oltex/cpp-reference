#pragma once

namespace library {
	template <typename type>
	class guard {
	protected:
		type* _value;
	public:
		inline explicit guard(type* value) noexcept
			: _value(value) {
			_value.guard_acquire();
		};
		inline explicit guard(guard const&) noexcept = delete;
		inline explicit guard(guard&&) noexcept = delete;
		inline auto operator=(guard const&) noexcept -> guard & = delete;
		inline auto operator=(guard&&) noexcept -> guard & = delete;
		inline ~guard(void) noexcept {
			if (nullptr != _value)
				_value.guard_release();
		};

		inline void release(void) noexcept {
			_value = nullptr;
		}
	};
}