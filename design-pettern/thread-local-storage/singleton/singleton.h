#pragma once
#include <type_traits>

namespace design_pattern {
	template<typename type>
	class local_static {
	public:
		inline static auto instance(void) noexcept -> type& {
			static type _instance;
			return _instance;
		}
	};
	template<typename type>
	class member_static {
	public:
		//inline static auto constructor(void) noexcept -> type&;
		inline static auto instance(void) noexcept -> type& {
			return *_instance;
		}
	protected:
		inline static type* _instance;
	};
	template<typename type>
	class global_static {
	public:
		inline static auto instance(void) noexcept -> type& {
			return _instance;
		}
	private:
		inline static type _instance;
	};

	template<typename type, typename trait = local_static<type>>
	class singleton : public trait {
	protected:
		inline explicit singleton(void) noexcept = default;
		inline ~singleton(void) noexcept = default;
	private:
		inline explicit singleton(singleton const& rhs) noexcept = delete;
		inline explicit singleton(singleton&& rhs) noexcept = delete;
		inline auto operator=(singleton const& rhs) noexcept -> singleton & = delete;
		inline auto operator=(singleton&& rhs) noexcept -> singleton & = delete;
	public:
		using trait::instance;
		inline static void destructor(void) noexcept requires std::is_same<member_static<type>, trait>::value {
			delete trait::_instance;
		}
	};
}