#pragma once
#include <utility>

namespace library {
	template<typename type_1, typename type_2, bool = std::is_empty_v<type_1> && !std::is_final_v<type_1>>
	struct compress_pair final : private type_1 {
		type_2 _second;
        inline auto first(void) noexcept -> type_1& {
            return *this;
        }
        inline auto second(void) noexcept -> type_2& {
            return _second;
        }
	};

    template <typename type_1, typename type_2>
    struct compress_pair<type_1, type_2, false> final {
        type_1 _first;
        type_2 _second;
        inline auto first(void) noexcept -> type_1& {
            return _first;
        }
        inline auto second(void) noexcept -> type_2& {
            return _second;
        }
    };
}