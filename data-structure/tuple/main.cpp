#include "../my_class.h"
#include <iostream>
#include <tuple>
#include "tuple.h"
int main(void) noexcept {
	std::tuple<my_class, my_class, my_class> std_tuple(1, 2, 3);
	std::tuple<my_class, my_class, my_class> std_tuple2(1, 2, 3);

	data_structure::tuple<my_class, my_class, my_class> tuple(1, 2, 3);
	data_structure::tuple<my_class, my_class, my_class> tuple2(2, 3, 4);

	std::get<2>(std_tuple);
	//std::get<1>(tuple);
	return 0;
}