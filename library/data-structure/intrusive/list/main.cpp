#include "list.h"
#include <iostream>

class my_class : public library::intrusive::list_hook<0>, public library::intrusive::list_hook<1> {
public:
	inline explicit my_class(int a) noexcept
		: _a(a) {
	}
public:
	int _a = 0;
};
int main(void) noexcept {
	my_class myclass1(1);

	library::intrusive::list<my_class, 0> list1;
	library::intrusive::list<my_class, 1> list2;
	list1.push_back(myclass1);
	list2.push_back(myclass1);

	//virtual »ó¼Ó


	//list1.swap(list2);

	//auto& a = list1.front();
	//std::cout << list1.front()._a << std::endl;
	//list1.erase(list1.begin());

	//std::cout << "list1 : " << std::endl;
	//for (auto& iter : list1)
	//	std::cout << iter._a << std::endl;
	//std::cout << "list2 : " << std::endl;
	//for (auto& iter : list2)
	//	std::cout << iter._a << std::endl;
	return 0;
}