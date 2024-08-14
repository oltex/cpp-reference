#pragma once

template<typename type>
class singleton {
protected:
	inline explicit singleton(void) noexcept = default;
	inline virtual ~singleton(void) noexcept = default;
private:
	inline explicit singleton(singleton const& rhs) noexcept = delete;
	inline auto operator=(singleton const& rhs) noexcept -> singleton& = delete;
	inline explicit singleton(singleton&& rhs) noexcept = delete;
	inline auto operator=(singleton&& rhs) noexcept -> singleton & = delete;
public:
	inline static type& instance(void) noexcept {
		static type inst;
		return inst;
	}
};