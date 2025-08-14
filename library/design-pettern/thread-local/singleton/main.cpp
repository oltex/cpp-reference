#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "singleton.h"
#include <iostream>

class my_class  {
private:
	friend class library::design_pattern::_thread_local::singleton<my_class>;
private:
	inline explicit my_class(void) noexcept = default;
	inline virtual ~my_class(void) noexcept = default;
	inline explicit my_class(my_class const& rhs) noexcept = delete;
	inline auto operator=(my_class const& rhs) noexcept -> my_class & = delete;
	inline explicit my_class(my_class&& rhs) noexcept = delete;
	inline auto operator=(my_class&& rhs) noexcept -> my_class & = delete;
public:
	inline void test(void) noexcept {
		std::cout << "hihi" << std::endl;
	}
};

int main(void) noexcept {
	auto& a = library::design_pattern::_thread_local::singleton<my_class>::instance();
	a.test();
	//my_class::instance();
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	return 0;
}