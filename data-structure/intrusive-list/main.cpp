#include "intrusive_list.h"
#include <iostream>
class my_class : public data_structure::intrusive_list_hook<0>, public data_structure::intrusive_list_hook<1> {
public:
	inline explicit my_class(int a) noexcept
		: _a(a) {
	}
public:
	int _a = 0;
};

class a {
public:
	int _a = 1;
};
class b : public a {
public:
	int _b = 2;
};

int main(void) noexcept {


	data_structure::intrusive_list<my_class, 1> list1;
	data_structure::intrusive_list<my_class, 1> list2;
	my_class myclass1(1);
	my_class myclass2(2);
	my_class myclass3(3);
	my_class myclass4(4);
	my_class myclass5(5);
	my_class myclass6(6);
	my_class myclass7(7);
	my_class myclass8(8);
	my_class myclass9(9);


	list1.push_back(myclass1);
	list1.push_back(myclass2);
	list1.push_back(myclass3);
	list1.push_back(myclass4);
	list1.push_back(myclass5);
	list1.push_back(myclass6);
	list1.erase(myclass3);
	list2.push_back(myclass7);
	list2.push_back(myclass8);
	list2.push_back(myclass9);
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