#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

#include "../my_class.h"
#include "vector.h"

#include <iostream>
#include <algorithm>
#include <vector>

union my_str
{
	unsigned long long tt;
	struct {
		unsigned long long _index : 16;
		unsigned long long _id : 48;
	} _t;
};
struct my_str2 {
	inline my_str2(unsigned long long a) noexcept {
		_index = 0xffff;
		_id = 0xffff;
		std::cout << "гоюл" << std::endl;
	}
	unsigned long long _index : 16;
	unsigned long long _id : 48;
};
struct my_str3 {
	unsigned long long _index : 16;
	unsigned long long _id : 48;
};


struct sss {
	int a;
};

int main(void) noexcept {
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	unsigned long long a{};
	//std::
	int result = ((my_str2*)&a)->_id;
	//int result2 = ((my_str3)a)._id;

	std::vector<my_class> std_vector;
	jun::vector<my_class> vector;
	vector.emplace_back(2);
	vector.emplace_back(2);
	vector.emplace_back(2);
	vector.emplace_back(2);
	vector.emplace_back(2);
	vector.resize(3, 1);
	return 0;
}