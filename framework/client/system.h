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
		using size_type = unsigned int;
		inline static size_type _type_id = 0;
		library::vector<library::tuple<argument*...>> _component;
	public:
		inline explicit system(void) noexcept = default;
		inline explicit system(system const&) noexcept = delete;
		inline explicit system(system&&) noexcept = delete;
		inline auto operator=(system const&) noexcept -> system & = delete;
		inline auto operator=(system&&) noexcept -> system & = delete;
		inline virtual ~system(void) noexcept = default;

		inline void add_component() noexcept {

		}
		inline virtual void update(void) noexcept = 0;

		template<typename type>
		inline static auto type_id(void) noexcept -> size_type {
			static size_type type_id = _type_id++;
			return type_id;
		}
	};
}