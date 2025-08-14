#pragma once
#include "../../intrusive/list.h"
#include <iostream>

namespace example::intrusive {
	inline void list(void) noexcept {
		class hook_class : public library::intrusive::list_hook<0>, public library::intrusive::list_hook<1> {
		public:
			inline explicit hook_class(int a) noexcept
				: _a(a) {
			}
			int _a = 0;
		};
		hook_class hookclass(1);

		library::intrusive::list<hook_class, 0> list1;
		library::intrusive::list<hook_class, 1> list2;
		list1.push_back(hookclass);
		list2.push_back(hookclass);

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
	}
}