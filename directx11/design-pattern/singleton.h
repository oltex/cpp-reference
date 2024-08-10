#pragma once

template<typename _Ty>
class singleton {
protected:
	inline explicit singleton(void) noexcept = default;
	inline virtual ~singleton(void) noexcept = default;
private:
	inline explicit singleton(singleton const& rhs) noexcept = delete;
	inline singleton& operator=(singleton const& rhs) noexcept = delete;
public:
	inline static _Ty& instance(void) noexcept {
		static _Ty inst;
		return inst;
	}
};