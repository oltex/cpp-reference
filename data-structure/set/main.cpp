#include <set>
#include "set.h"
#include "../my_class.h"

int main(void) noexcept {
	library::data_structure::set<my_class> set;

	auto iter = set.emplace(my_class(1));
	set.emplace(2);
	return 0;
}