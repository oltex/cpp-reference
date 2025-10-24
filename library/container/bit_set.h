#pragma once
#include <type_traits>
#include "../memory.h"

//class reference { // proxy for an element
//	friend bitset;
//
//public:
//	_CONSTEXPR23 reference(const reference&) = default;
//
//	_CONSTEXPR23 ~reference() noexcept {} // TRANSITION, ABI
//
//	_CONSTEXPR23 reference& operator=(const bool _Val) noexcept {
//		_Pbitset->_Set_unchecked(_Mypos, _Val);
//		return *this;
//	}
//
//	_CONSTEXPR23 reference& operator=(const reference& _Bitref) noexcept {
//		_Pbitset->_Set_unchecked(_Mypos, static_cast<bool>(_Bitref));
//		return *this;
//	}
//
//	_NODISCARD _CONSTEXPR23 bool operator~() const noexcept {
//		return !_Pbitset->_Subscript(_Mypos);
//	}
//
//	_CONSTEXPR23 operator bool() const noexcept {
//		return _Pbitset->_Subscript(_Mypos);
//	}
//
//	_CONSTEXPR23 reference& flip() noexcept {
//		_Pbitset->_Flip_unchecked(_Mypos);
//		return *this;
//	}
//
//private:
//	_CONSTEXPR23 reference() noexcept : _Pbitset(nullptr), _Mypos(0) {}
//
//	_CONSTEXPR23 reference(bitset<_Bits>& _Bitset, const size_t _Pos) noexcept : _Pbitset(&_Bitset), _Mypos(_Pos) {}
//
//	bitset<_Bits>* _Pbitset;
//	size_t _Mypos; // position of element in bitset
//};

namespace library {
	template<size_t size>
	class bit_set {
		using size_type = unsigned int;
		using type = std::conditional_t<size <= sizeof(unsigned long) * 8, unsigned long, unsigned long long>;
		inline static constexpr size_type bit = sizeof(type) * 8;
		inline static constexpr size_type word = (size - 1) / bit + 1;
		type _array[word];
	public:
		inline explicit bit_set(void) noexcept
			: _array{} {
		};
		inline bit_set(bit_set const&) noexcept = default;
		inline bit_set(bit_set&&) noexcept = default;
		inline auto operator=(bit_set const& rhs) noexcept -> bit_set & = default;
		inline auto operator=(bit_set&& rhs) noexcept -> bit_set & = default;
		inline ~bit_set(void) noexcept = default;

		inline void set(size_type const position, bool const value) noexcept {
			if (false == value)
				_array[position / bit] &= ~(static_cast<type>(1) << position % bit);
			else
				_array[position / bit] |= static_cast<type>(1) << position % bit;
		}
		inline void flip(size_type const position) noexcept {
			_array[position / bit] ^= static_cast<type>(1) << position % bit;
			return *this;
		}
		inline void reset(void) noexcept {
			library::memory_set(_array, 0, sizeof(_array));
		}
		inline auto test(size_type const position) const noexcept -> bool {
			return 0 != (_array[position / bit] & static_cast<type>(1) << position % bit);

		}
		inline auto operator==(bit_set const& rhs) const noexcept -> bool {
			return library::memory_compare(_array, rhs._array, word);
		}
		inline auto operator&=(bit_set const& rhs) noexcept -> bit_set& {
			for (size_type index = 0; index < word; ++index)
				_array[index] &= rhs._array[index];
			return *this;
		}
		inline auto operator|=(bit_set const& rhs) noexcept -> bit_set& {
			for (size_type index = 0; index < word; ++index)
				_array[index] |= rhs._array[index];
			return *this;
		}
		inline auto operator^=(bit_set const& rhs) noexcept  -> bit_set& {
			for (size_type index = 0; index < word; ++index)
				_array[index] ^= rhs._array[index];
			return *this;
		}
		inline auto operator&(bit_set const& rhs) const noexcept -> bit_set {
			bit_set result(*this);
			result &= rhs;
			return result;
		}
		inline auto operator|(bit_set const& rhs) const noexcept -> bit_set {
			bit_set result(*this);
			result |= rhs;
			return result;
		}
		inline auto operator^(bit_set const& rhs) const noexcept -> bit_set {
			bit_set result(*this);
			result ^= rhs;
			return result;
		}
		inline auto operator~(void) const noexcept -> bit_set {
			bit_set result(*this);
			for (size_type index = 0; index < word; ++index)
				result._array[index] = ~result._array[index];
			return result;
		}
		inline explicit operator bool(void) const noexcept {
			for (size_type index = 0; index < word; ++index)
				if (_array[index] != 0)
					return true;
			return false;
		}
	};
}