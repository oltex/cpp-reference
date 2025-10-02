#pragma once
#include "library/container/intrusive/pointer_list.h"
#include "library/container/list.h"

namespace framework {
	class component;
	class system : public library::intrusive::pointer_hook<0>, public library::intrusive::list_hook<0> {
		using size_type = unsigned int;
	public:
		explicit system(void) noexcept = default;
		explicit system(system const&) noexcept = delete;
		explicit system(system&&) noexcept = delete;
		auto operator=(system const&) noexcept -> system & = delete;
		auto operator=(system&&) noexcept -> system & = delete;
		virtual ~system(void) noexcept = default;

		virtual void update(void) noexcept = 0;
		//virtual void add_component(library::list<component*> component) noexcept = 0;

		template<size_t index>
		inline static void deallocate(system* pointer) noexcept {};
		template<>
		inline static void deallocate<0>(system* pointer) noexcept {
			delete pointer;
		}
	};
}