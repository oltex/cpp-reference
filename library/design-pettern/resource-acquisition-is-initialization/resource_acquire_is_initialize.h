#pragma once

namespace library {
	template <typename type>
	class resource_acquire_is_initialize {
		type& _value;
	public:
		inline explicit resource_acquire_is_initialize(type& value) noexcept 
			: _value(value) {
		};
		inline explicit resource_acquire_is_initialize(resource_acquire_is_initialize const&) noexcept = delete;
		inline explicit resource_acquire_is_initialize(resource_acquire_is_initialize&&) noexcept = delete;
		inline auto operator=(resource_acquire_is_initialize const&) noexcept -> resource_acquire_is_initialize & = delete;
		inline auto operator=(resource_acquire_is_initialize&&) noexcept -> resource_acquire_is_initialize & = delete;
		inline ~resource_acquire_is_initialize(void) noexcept = default;
	};
}