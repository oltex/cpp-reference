#pragma once
#include <utility>
#include "A.h"

class A;

class B {
public:
	void func(A& _A) {
		_A._a = 20;
	}

	void chage(int b) {
		_b = b;
	}
	int _b = 20;
};