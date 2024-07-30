#pragma once

template<class _Kty>
struct hash final {
    inline size_t operator()(_Kty const& key) const noexcept {
        size_t val = 14695981039346656037ULL;
        unsigned char const* const byte = &reinterpret_cast<unsigned char const&>(key);
        for (size_t idx = 0; idx < sizeof(_Kty); ++idx) {
            val ^= static_cast<size_t>(byte[idx]);
            val *= 1099511628211ULL;
        }
        return val;
    }
};