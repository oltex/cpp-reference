#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

//#include "example/list.h"
//#include "example/pool.h"
//#include "example/pair.h"
//#include "example/tuple.h"
//#include "example/vector.h"
//#include "example/stack.h"
//#include "example/queue.h"
//#include "example/compress_pair.h"
//#include "example/array.h"
//#include "example/priority_queue.h"
//#include "example/serialize_buffer.h"
//#include "example/string.h"
//#include "example/storage.h"
//#include "example/unique_pointer.h"
//#include "example/share_pointer.h"
//#include "example/weak_pointer.h"
//#include "example/circle_queue.h"
//#include "example/bit_grid.h"
//#include "example/ring_buffer.h"
//#include "example/intrusive/list.h"
//#include "example/intrusive/share_pointer.h"
//#include "example/lockfree/stack.h"
//#include "example/lockfree/pool.h"
//#include "example/lockfree/queue.h"
//#include "example/thread-local/pool.h"
//#include "example/unorder_map.h"
#include "example/hash_table.h"

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	//example::list();
	//example::pair();
	//example::tuple();
	//example::pool();
	//example::vector();
	//example::stack();
	//example::queue();
	//example::compress_pair();
	//example::array();
	//example::priority_queue();
	//example::serialize_buffer();
	//example::string();
	//example::stroage();
	//example::unique_pointer();
	//example::share_pointer();
	//example::weak_pointer();
	//example::circle_queue();
	//example::bit_grid();
	//example::ring_buffer();
	//example::intrusive::list();
	//example::intrusive::share_pointer();
	//example::lockfree::stack();
	//example::lockfree::pool();
	//example::lockfree::queue();
	//example::_thread_local::pool();
	//example::unorder_map();
	example::hash_table();

	return 0;
}