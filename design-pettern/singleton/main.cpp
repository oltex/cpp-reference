#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "singleton.h"
#include <iostream>

class my_class : public design_pattern::singleton<my_class, design_pattern::member_static<my_class>> {
	friend class design_pattern::singleton<my_class, design_pattern::member_static<my_class>>;
public:
	inline static auto constructor(void) noexcept -> my_class& {
		_instance = new my_class();
		atexit(destructor);
		return *_instance;
	}
private:
	inline explicit my_class(void) noexcept = default;
	inline virtual ~my_class(void) noexcept = default;
	inline explicit my_class(my_class const& rhs) noexcept = delete;
	inline auto operator=(my_class const& rhs) noexcept -> my_class & = delete;
	inline explicit my_class(my_class&& rhs) noexcept = delete;
	inline auto operator=(my_class&& rhs) noexcept -> my_class & = delete;
};

void func(void) noexcept {
	my_class::constructor();
	my_class::instance();
}

int main(void) noexcept {
	func();
	_CrtDumpMemoryLeaks();
	return 0;
}