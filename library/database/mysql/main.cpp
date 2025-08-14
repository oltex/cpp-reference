#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "mysql.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	database::mysql::initialize();

	database::mysql mysql;
	mysql.connect("127.0.0.1", "root", "1111", "new_schema", 3306);
	//mysql.query()

	database::mysql::end();
	return 0;
}