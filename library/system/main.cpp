#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "file.h"
//#include "example/interlock.h"
//#include "example/coroutine.h"
#include "type_info.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//example::interlock();
	//example::coroutine();

	return 0;
}