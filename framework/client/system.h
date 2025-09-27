#pragma once
#include "library/container/intrusive/list.h"

namespace framework {
	class system : public library::intrusive::list_hook<0> {
	protected:
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