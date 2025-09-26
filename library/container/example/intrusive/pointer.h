#pragma once
#include "../../intrusive/pointer.h"
#include <iostream>
#include <memory>


namespace example::intrusive {
	struct temp {
		inline virtual ~temp(void) noexcept {
		};

		int _temp = 0;
	};
	class base : public library::intrusive::pointer_hook<1> {
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

		template<size_t index>
		inline static void deallocate(base* ptr) noexcept;
		template<>
		inline static void deallocate<1>(base* ptr) noexcept {
			delete ptr;
		}
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


	inline void pointer(void) noexcept {
		//auto pointer = new derive(10, 20);
		//library::intrusive::share_pointer<derive, 1> pointer2(pointer);
		//library::intrusive::weak_pointer<base, 1> pointer3(std::move(pointer2));

		library::intrusive::share_pointer<derive, 1> pointer(new derive(10, 20));
		library::intrusive::share_pointer<base, 1> pointer2 = pointer;

		library::intrusive::weak_pointer<base, 1> pointer3(pointer);
		library::intrusive::weak_pointer<derive, 1> pointer4(pointer);
		library::intrusive::weak_pointer<derive, 1> pointer5(pointer4);
		//auto a intrusive= pointer2.get();
		//auto b = pointer3.get();
		//int c = 10;
	}
}