#pragma once
#include "../singleton.h"
#include <iostream>

namespace example {
	class my_class : public library::singleton<my_class, library::member_static<my_class>> {
		friend class library::singleton<my_class, library::member_static<my_class>>;
	public:
		inline static auto constructor(void) noexcept -> my_class& {
			_instance = new my_class();
			atexit(destructor);
			return *_instance;
		}
	private:
		inline explicit my_class(void) noexcept = default;
		inline explicit my_class(my_class const& rhs) noexcept = delete;
		inline explicit my_class(my_class&& rhs) noexcept = delete;
		inline auto operator=(my_class const& rhs) noexcept -> my_class & = delete;
		inline auto operator=(my_class&& rhs) noexcept -> my_class & = delete;
		inline ~my_class(void) noexcept = default;
	};

	inline void singleton(void) noexcept {
		my_class::constructor();
		my_class::instance();
	}
}