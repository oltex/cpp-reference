#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "pair.h"
#include "../tuple/tuple.h"
#include "../my_class.h"
#include <tuple>

struct test {

};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	library::pair<int, test> pair2;


	std::pair<int,int>(std::piecewise_construct, std::forward_as_tuple(10), std::forward_as_tuple(10));
	library::pair<my_class, my_class> pair(library::piecewise_construct, library::forward_as_tuple(10), library::forward_as_tuple(10));

	//pair._first = 0;
	//pair._second = 1;

	return 0;
}