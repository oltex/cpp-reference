#pragma once
#include <iostream>

namespace example {
	struct temp {
		inline virtual ~temp(void) noexcept {
		};

		int _temp = 0;
	};
	class base {
	public:
		int _base_value;
		inline explicit base(int base_value) noexcept
			: _base_value(base_value) {
			std::cout << "base 생성자: " << _base_value << std::endl;
		}
		inline base(base const& rhs) noexcept
			: _base_value(rhs._base_value) {
			std::cout << "base 복사 생성자" << std::endl;
		}
		inline base(base&& rhs) noexcept
			: _base_value(rhs._base_value) {
			std::cout << "base 이동 생성자" << std::endl;
		}
		inline base& operator=(base const& rhs) noexcept {
			_base_value = rhs._base_value;
			std::cout << "base 복사 대입 연산자" << std::endl;
			return *this;
		};
		inline base& operator=(base&& rhs) noexcept {
			_base_value = rhs._base_value;
			std::cout << "base 이동 대입 연산자" << std::endl;
			return *this;
		}
		inline virtual ~base(void) noexcept {
			std::cout << "base 소멸자: " << _base_value << std::endl;
		};
	};
	class derive : public temp, public base {
	public:
		int _derive_value;
		inline explicit derive(int base_value, int derive_value) noexcept
			: base(base_value), _derive_value(derive_value) {
			std::cout << "derive 생성자: " << _derive_value << std::endl;
		}
		inline derive(derive const& rhs) noexcept
			: base(rhs), _derive_value(rhs._derive_value) {
			std::cout << "derive 복사 생성자" << std::endl;
		}
		inline derive(derive&& rhs) noexcept
			: base(std::move(rhs)), _derive_value(rhs._derive_value) {
			std::cout << "derive 이동 생성자" << std::endl;
		}
		inline derive& operator=(derive const& rhs) noexcept {
			_derive_value = rhs._derive_value;
			std::cout << "derive 복사 대입 연산자" << std::endl;
			return *this;
		};
		inline derive& operator=(derive&& rhs) noexcept {
			_derive_value = rhs._derive_value;
			std::cout << "derive 이동 대입 연산자" << std::endl;
			return *this;
		}
		inline virtual ~derive(void) noexcept override {
			std::cout << "derive 소멸자: " << _derive_value << std::endl;
		};
	};
	class my_class {
	public:
		inline explicit my_class(int value) noexcept
			: _value(value) {
			std::cout << "생성자: " << _value << std::endl;
		}
		inline my_class(const my_class& rhs) noexcept
			: _value(rhs._value) {
			std::cout << "복사 생성자" << std::endl;
		}
		inline my_class(my_class&& rhs) noexcept
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
		inline ~my_class(void) noexcept {
			std::cout << "소멸자: " << _value << std::endl;
		};
		inline bool operator == (const my_class& other) const noexcept {
			if (_value == other._value)
				return true;
			return false;
		}
		inline bool operator < (const my_class& other) const noexcept {
			if (_value < other._value)
				return true;
			return false;
		}
		inline auto operator <=> (const my_class& other) const noexcept {
			return _value <=> other._value;
		}

		inline void function(void) noexcept {
			std::cout << "함수: " << _value << std::endl;
		}
		int _value;
	};
}