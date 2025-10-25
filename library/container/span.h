#pragma once

namespace detail {
    template <typename type, size_t size>
    class span {
    protected:
        type* _pointer;
        inline static constexpr size_t _size = size;
    public:
        inline constexpr span(void) noexcept = default;

        inline constexpr span(type(&pointer)[size]) noexcept
            : _pointer(pointer) {
        }
        inline constexpr explicit span(type* pointer, size_t const) noexcept 
            : _pointer(pointer) {
        }
    };

    template <typename type>
    struct span<type, 0xFFFFFFFFFFFFFFFFULL> {
    protected:
        type* _pointer;
        size_t _size;
    public:
        inline constexpr span(void) noexcept 
            : _pointer(nullptr), _size(0) {
        };
        inline constexpr explicit span(type* pointer, size_t const size) noexcept
            : _pointer(pointer), _size(size) {
        }
    };
}

namespace library {
    template <typename type, size_t size = 0xFFFFFFFFFFFFFFFFULL>
	class span : detail::span<type, size> {
        using base = detail::span<type, size>;
        using size_type = unsigned int;
    public:
        using base::base;
        inline constexpr auto size(void) noexcept {
            return base::_size;
        }
	};
}