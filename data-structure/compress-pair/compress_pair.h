#pragma once
#include <utility>

namespace detail {
    template<typename type_1, typename type_2, bool = std::is_same_v<type_1, type_2>, bool = std::is_empty_v<type_1> && !std::is_final_v<type_1>, bool = std::is_empty_v<type_2> && !std::is_final_v<type_2>>
    struct compress_pair {
    };
    template <typename type_1, typename type_2, bool same>
    struct compress_pair<type_1, type_2, same, true, false> : private type_1 {
        type_2 _second;
        inline auto first(void) noexcept -> type_1& {
            return *this;
        }
        inline auto second(void) noexcept -> type_2& {
            return _second;
        }
    };
    template <typename type_1, typename type_2, bool same>
    struct compress_pair<type_1, type_2, same, false, true> : private type_2 {
        type_1 _first;
        inline auto first(void) noexcept -> type_1& {
            return _first;
        }
        inline auto second(void) noexcept -> type_2& {
            return *this;
        }
    };
    template <typename type_1, typename type_2, bool same>
    struct compress_pair<type_1, type_2, same, false, false> {
        type_1 _first;
        type_2 _second;
        inline auto first(void) noexcept -> type_1& {
            return _first;
        }
        inline auto second(void) noexcept -> type_2& {
            return _second;
        }
    };
    template<typename type_1, typename type_2>
    struct compress_pair<type_1, type_2, true, true, true> : private type_1 {
        type_2 _second;
        inline auto first(void) noexcept -> type_1& {
            return *this;
        }
        inline auto second(void) noexcept -> type_2& {
            return _second;
        }
    };
    template<typename type_1, typename type_2>
    struct compress_pair<type_1, type_2, false, true, true> : private type_1, private type_2 {
        inline auto first(void) noexcept -> type_1& {
            return *this;
        }
        inline auto second(void) noexcept -> type_2& {
            return *this;
        }
    };
}

namespace library {
	template<typename type_1, typename type_2>
	struct compress_pair final : public detail::compress_pair<type_1, type_2> {
	};
}