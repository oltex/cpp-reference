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
			std::cout << "base ������: " << _base_value << std::endl;
		}
		inline base(base const& rhs) noexcept
			: _base_value(rhs._base_value) {
			std::cout << "base ���� ������" << std::endl;
		}
		inline base(base&& rhs) noexcept
			: _base_value(rhs._base_value) {
			std::cout << "base �̵� ������" << std::endl;
		}
		inline base& operator=(base const& rhs) noexcept {
			_base_value = rhs._base_value;
			std::cout << "base ���� ���� ������" << std::endl;
			return *this;
		};
		inline base& operator=(base&& rhs) noexcept {
			_base_value = rhs._base_value;
			std::cout << "base �̵� ���� ������" << std::endl;
			return *this;
		}
		inline virtual ~base(void) noexcept {
			std::cout << "base �Ҹ���: " << _base_value << std::endl;
		};
	};
	class derive : public temp, public base {
	public:
		int _derive_value;
		inline explicit derive(int base_value, int derive_value) noexcept
			: base(base_value), _derive_value(derive_value) {
			std::cout << "derive ������: " << _derive_value << std::endl;
		}
		inline derive(derive const& rhs) noexcept
			: base(rhs), _derive_value(rhs._derive_value) {
			std::cout << "derive ���� ������" << std::endl;
		}
		inline derive(derive&& rhs) noexcept
			: base(std::move(rhs)), _derive_value(rhs._derive_value) {
			std::cout << "derive �̵� ������" << std::endl;
		}
		inline derive& operator=(derive const& rhs) noexcept {
			_derive_value = rhs._derive_value;
			std::cout << "derive ���� ���� ������" << std::endl;
			return *this;
		};
		inline derive& operator=(derive&& rhs) noexcept {
			_derive_value = rhs._derive_value;
			std::cout << "derive �̵� ���� ������" << std::endl;
			return *this;
		}
		inline virtual ~derive(void) noexcept override {
			std::cout << "derive �Ҹ���: " << _derive_value << std::endl;
		};
	};
	class my_class {
	public:
		inline explicit my_class(int value) noexcept
			: _value(value) {
			std::cout << "������: " << _value << std::endl;
		}
		inline my_class(const my_class& rhs) noexcept
			: _value(rhs._value) {
			std::cout << "���� ������" << std::endl;
		}
		inline my_class(my_class&& rhs) noexcept
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
		inline ~my_class(void) noexcept {
			std::cout << "�Ҹ���: " << _value << std::endl;
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
			std::cout << "�Լ�: " << _value << std::endl;
		}
		int _value;
	};
}