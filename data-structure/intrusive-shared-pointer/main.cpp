#include "intrusive_shared_pointer.h"
#include <iostream>
#include <memory>

class my_class final : public data_structure::intrusive_shared_pointer_hook<1> {
public:
	inline explicit my_class(int value) noexcept
		: _value(value) {
		std::cout << "생성자" << std::endl;
	}
	inline my_class(const my_class& rhs) noexcept
		: _value(rhs._value) {
		std::cout << "복사 생성자" << std::endl;
	}
	inline explicit my_class(my_class&& rhs) noexcept
		: _value(rhs._value) {
		std::cout << "이동 생성자" << std::endl;
	}
	inline my_class& operator=(const my_class& rhs) noexcept {
		_value = rhs._value;
		std::cout << "복사 대입 연산자" << std::endl;
		return *this;
	};
	inline my_class& operator=(my_class&& rhs) noexcept {
		_value = rhs._value;
		std::cout << "이동 대입 연산자" << std::endl;
		return *this;
	}
	inline bool operator == (const my_class& other) const noexcept {
		if (_value == other._value)
			return true;
		return false;
	}
	inline ~my_class(void) noexcept {
		std::cout << "소멸자" << std::endl;
	};
public:
	//friend void intrusive_ptr_add_ref(MyObject* p) {
	//	++p->ref_count;
	//}

	//friend void intrusive_ptr_release(MyObject* p) {
	//	if (--p->ref_count == 0) {
	//		delete p;
	//	}
	//}
	void temp(void) noexcept {
		int a = 10;
	}
public:
	int _value;
};

class test {
public:
	void func(void) noexcept {

	}
	friend void func2(test * t) noexcept {
	}
private:
	int value = 10;
};

int main(void) noexcept {
	my_class* myclass = new my_class(1);
	data_structure::intrusive_shared_pointer<my_class, 1> shared_pointer(myclass);



	auto myclass2 = shared_pointer.data();
	shared_pointer.reset();
	delete myclass2;

	return 0;
}