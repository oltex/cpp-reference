#pragma once

namespace library {
	template <class type, class other = type>
	inline constexpr type exchange(type& value, other&& new_value) noexcept {
		type old_value = static_cast<type&&>(value);
		value = static_cast<other&&>(new_value);
		return old_value;
	}
}