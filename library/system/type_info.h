#pragma once
#include "../function.h"
#include "../container/string.h"

namespace library {
	class type_base {
	public:
		inline virtual auto type_name(void) const noexcept -> char const* const {
			return nullptr;
		}
		inline virtual auto type_hash(void) const noexcept -> unsigned int {
			return 0;
		}
	};

	template<typename base, library::string_literal name>
	class type_derive : public base {
		struct type_info {
			const char* _name;
			unsigned int _id;
		};
		inline static constexpr type_info _info{ name._value, fnv_hash(name._vaule) };
	public:
		using base::base;
		inline virtual auto type_name(void) const noexcept -> char const* const override {
			return _info._name;
		}
		inline virtual auto type_hash(void) const noexcept -> unsigned int override {
			return _info._id;
		}
	};
}