#pragma once
//#include "library/container/tuple.h"
//#include "library/container/vector.h"
//
//class system_interface {
//public:
//	inline virtual void update(void) noexcept = 0;
//};
//
//library::vector<library::tuple<argument*...>> _component;
//template <typename... argument>
#include "library/container/intrusive/list.h"

namespace framework {
	class system : public library::intrusive::list_hook<0> {
	protected:
		using size_type = unsigned int;
		inline static size_type _type_id = 0;
	public:
		inline explicit system(void) noexcept = default;
		inline explicit system(system const&) noexcept = delete;
		inline explicit system(system&&) noexcept = delete;
		inline auto operator=(system const&) noexcept -> system & = delete;
		inline auto operator=(system&&) noexcept -> system & = delete;
		inline virtual ~system(void) noexcept = default;

		inline virtual void update(void) noexcept = 0;

		template<typename type>
		inline static auto type_id(void) noexcept -> size_type {
			static size_type type_id = _type_id++;
			return type_id;
		}
	};
}