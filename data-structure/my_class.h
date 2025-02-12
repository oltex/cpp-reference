#pragma once
#include <iostream>

class my_class final {
public:
	inline explicit my_class(int value) noexcept
		: _value(value) {
		std::cout << "������" << std::endl;
	}
	inline my_class(const my_class& rhs) noexcept
		: _value(rhs._value) {
		std::cout << "���� ������" << std::endl;
	}
	inline explicit my_class(my_class&& rhs) noexcept
		: _value(rhs._value) {
		std::cout << "�̵� ������" << std::endl;
	}
	inline my_class& operator=(const my_class& rhs) noexcept {
		_value = rhs._value;
		std::cout << "���� ���� ������" << std::endl;
		return *this;
	};
	inline my_class& operator=(my_class&& rhs) noexcept {
		_value = rhs._value;
		std::cout << "�̵� ���� ������" << std::endl;
		return *this;
	}
	inline bool operator == (const my_class& other) const noexcept {
		if (_value == other._value)
			return true;
		return false;
	}
	inline ~my_class(void) noexcept {
		std::cout << "�Ҹ���" << std::endl;
	};
public:
	inline void function(void) noexcept {
		std::cout << "�Լ�" << std::endl;
	}
private:
	int _value;
};