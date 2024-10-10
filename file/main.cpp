#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "file.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	file file;
	file.create(L"test.txt", FILE_WRITE_DATA, FILE_SHARE_READ, CREATE_ALWAYS, FILE_FLAG_WRITE_THROUGH);
	file.write("Hello", 5);
	file.set_pointer(2, FILE_BEGIN);
	file.set_end();

	return 0;
}