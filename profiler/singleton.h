#pragma once

template<typename type>
class singleton {
protected:
	inline explicit singleton(void) noexcept = default;
	virtual ~singleton(void) noexcept = default;
private:
	inline explicit singleton(singleton const& rhs) noexcept = delete;
	inline singleton& operator=(singleton const& rhs) noexcept = delete;
public:
	inline static type& instance(void) noexcept {
		static type inst;
		return inst;
	}
};