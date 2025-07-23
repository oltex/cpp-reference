#pragma once
#include <utility>

class B;

class A {
public:
	void func(B& _B) {
		_B.chage(10);
		//_B._b = 10;
	}
	int _a = 10;
};