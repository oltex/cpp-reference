#pragma once
#include "library/container/intrusive/pointer_list.h"

namespace framework {
	class system : public library::intrusive::pointer_hook<0>, public library::intrusive::list_hook<0> {
	protected:
		friend class library::intrusive::share_pointer<system, 0>;
		friend class library::intrusive::weak_pointer<system, 0>;
	
		template<size_t index>
		inline static void deallocate(system* pointer) noexcept {};
		template<>
		inline static void deallocate<0>(system* pointer) noexcept {
			delete pointer;
		}

		using size_type = unsigned int;
	public:
		explicit system(void) noexcept = default;
		explicit system(system const&) noexcept = delete;
		explicit system(system&&) noexcept = delete;
		auto operator=(system const&) noexcept -> system & = delete;
		auto operator=(system&&) noexcept -> system & = delete;
		virtual ~system(void) noexcept = default;

		virtual void update(void) noexcept = 0;
	};
}