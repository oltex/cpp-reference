#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "singleton.h"
#include <iostream>

class my_class : public library::design_pattern::singleton<my_class, library::design_pattern::member_static<my_class>> {
	friend class library::design_pattern::singleton<my_class, library::design_pattern::member_static<my_class>>;
public:
	inline static auto constructor(void) noexcept -> my_class& {
		_instance = new my_class();
		atexit(destructor);
		return *_instance;
	}
private:
	inline explicit my_class(void) noexcept = default;
	inline explicit my_class(my_class const& rhs) noexcept = delete;
	inline explicit my_class(my_class&& rhs) noexcept = delete;
	inline auto operator=(my_class const& rhs) noexcept -> my_class & = delete;
	inline auto operator=(my_class&& rhs) noexcept -> my_class & = delete;
	inline ~my_class(void) noexcept = default;
};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	my_class::constructor();
	my_class::instance();
	return 0;
}