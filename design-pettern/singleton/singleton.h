#pragma once
#include <type_traits>

namespace design_pattern {
	template<typename type>
	class local_static {
	};
	template<typename type>
	class member_static {
		//inline static auto constructor(void) noexcept -> type&;
	protected:
		inline static type* _instance;
	};
	template<typename type>
	class global_static {
	private:
		inline static type _instance;
	};
	template<typename type>
	class member_static_thread_safe {
	private:
		inline static type* _instance;
	};

	template<typename type, typename trait = local_static<type>>
	class singleton : public trait {
	protected:
		inline explicit singleton(void) noexcept = default;
		inline ~singleton(void) noexcept = default;
	private:
		inline explicit singleton(singleton const&) noexcept = delete;
		inline explicit singleton(singleton&&) noexcept = delete;
		inline auto operator=(singleton const&) noexcept -> singleton & = delete;
		inline auto operator=(singleton&&) noexcept -> singleton & = delete;
	public:
		inline static auto instance(void) noexcept -> type& requires std::is_same<local_static<type>, trait>::value {
			static type _instance;
			return _instance;
		}
		inline static auto instance(void) noexcept -> type& requires std::is_same<member_static<type>, trait>::value {
			return *trait::_instance;
		}
		inline static void destructor(void) noexcept requires std::is_same<member_static<type>, trait>::value {
			delete trait::_instance;
		}
		inline static auto instance(void) noexcept -> type& requires std::is_same<global_static<type>, trait>::value {
			return trait::_instance;
		}
	};
}