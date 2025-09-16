#pragma once
#include "library/container/tuple.h"
#include "library/container/vector.h"

namespace framework {
	class system_interface {
		inline virtual void update(void) noexcept = 0;
	};

	template <typename... argument>
	class system : public system_interface {
	protected:
		library::vector<library::tuple<argument*...>> _component;
	public:
		inline explicit system(void) noexcept = default;
		inline explicit system(system const&) noexcept = delete;
		inline explicit system(system&&) noexcept = delete;
		inline auto operator=(system const&) noexcept -> system & = delete;
		inline auto operator=(system&&) noexcept -> system & = delete;
		inline virtual ~system(void) noexcept = default;

		inline virtual void update(void) noexcept = 0;
	};
}