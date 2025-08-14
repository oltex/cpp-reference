#pragma once
#include "command.h"
#include <iostream>

class my_class {
public:
	void pirnt(void) noexcept {
		std::cout << "hello" << std::endl;
	}
	int member(command::parameter* par) noexcept {
		std::cout << "member" << _session << std::endl;
		return 0;
	}
private:
	int _session = 10;
};

int main(void) noexcept {
	//bind
	my_class mycls;
	command c([&mycls](command::parameter*) -> int {
		mycls.pirnt();
		return 0;
		});
	command c2(std::bind(&my_class::member, &mycls, std::placeholders::_1));


	//execute
	command::parameter* p = new command::parameter;
	c.execute(p);
	c2.execute(p);
	return 0;
}