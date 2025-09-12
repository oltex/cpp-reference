#pragma once
#include "../memory.h"

namespace library {
	template<typename type, bool placement = false>
	class singleton {
		inline explicit singleton(singleton const&) noexcept = delete;
		inline explicit singleton(singleton&&) noexcept = delete;
		inline auto operator=(singleton const&) noexcept -> singleton & = delete;
		inline auto operator=(singleton&&) noexcept -> singleton & = delete;
	protected:
		inline explicit singleton(void) noexcept = default;
		inline ~singleton(void) noexcept = default;
	public:
		inline static auto instance(void) noexcept -> type& {
			static type _instance;
			return _instance;
		}
	};
	template<typename type>
	class singleton<type, true> {
		inline static type* _instance;

		inline explicit singleton(singleton const&) noexcept = delete;
		inline explicit singleton(singleton&&) noexcept = delete;
		inline auto operator=(singleton const&) noexcept -> singleton & = delete;
		inline auto operator=(singleton&&) noexcept -> singleton & = delete;
	protected:
		inline explicit singleton(void) noexcept = default;
		inline ~singleton(void) noexcept = default;
	public:
		template<typename... argument>
		inline static auto construct(argument&&... arg) noexcept -> type& {
			_instance = library::allocate<type>();
			library::construct(*_instance, std::forward<argument>(arg)...);
			return *_instance;
		}
		inline static auto instance(void) noexcept -> type& {
			return *_instance;
		}
		inline static void destruct(void) noexcept {
			library::destruct(*_instance);
			library::deallocate(_instance);
		}
	};
}