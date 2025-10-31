#include "pool.h"

namespace framework {
	void pools::deallocate(base* pointer) noexcept {
		auto& result = _pool.find(pointer->type())->_second;
		result->deallocate(pointer);
	}
}
