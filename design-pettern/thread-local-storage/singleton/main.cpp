#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "singleton.h"
#include <iostream>

class my_class : public design_pattern::singleton<my_class> {
	friend class design_pattern::singleton<my_class>;
private:
	inline explicit my_class(void) noexcept = default;
	inline virtual ~my_class(void) noexcept = default;
	inline explicit my_class(my_class const& rhs) noexcept = delete;
	inline auto operator=(my_class const& rhs) noexcept -> my_class & = delete;
	inline explicit my_class(my_class&& rhs) noexcept = delete;
	inline auto operator=(my_class&& rhs) noexcept -> my_class & = delete;
};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	my_class::instance();
	return 0;
}