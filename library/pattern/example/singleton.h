#pragma once
#include "../singleton.h"
#include <iostream>


namespace example {
	class my_class : public library::singleton<my_class> {
		friend class library::singleton<my_class>;
	private:
		inline explicit my_class(void) noexcept = default;
		inline explicit my_class(my_class const& rhs) noexcept = delete;
		inline explicit my_class(my_class&& rhs) noexcept = delete;
		inline auto operator=(my_class const& rhs) noexcept -> my_class & = delete;
		inline auto operator=(my_class&& rhs) noexcept -> my_class & = delete;
		inline ~my_class(void) noexcept = default;
	};

	class a : public library::singleton<a> {
		friend class library::singleton<a>;
	public:
		inline a(void) noexcept {
			printf("a 持失切\n");
		}
		inline void function(void) noexcept {
			printf("a\n");
		}
	};

	class b :  public a, public library::singleton<b> {
		friend class library::singleton<b>;
	public:
		inline b(void) noexcept {
			printf("b 持失切\n");
		}
		inline void function(void) noexcept {
			printf("b\n");
		}
		//inline static auto instance(void) noexcept -> b& {
		//	return library::singleton<b>::instance();
		//}
		using library::singleton<b>::instance;
	};

	inline void singleton(void) noexcept {
		auto& inst = a::instance();
		inst.function();
		auto& inst2 = b::instance();
		inst2.function();
		auto& inst3 = library::singleton<b>::instance();
	}
}