#pragma once
#include "component.h"
#include "object.h"

namespace framework {
	class behaviour : public component {
	protected:
		library::intrusive::weak_pointer<object, 0> _parent;
	public:
		explicit behaviour(library::intrusive::share_pointer<object, 0>& parent) noexcept;
		explicit behaviour(behaviour const&) noexcept = delete;
		explicit behaviour(behaviour&&) noexcept = delete;
		auto operator=(behaviour const&) noexcept -> behaviour & = delete;
		auto operator=(behaviour&&) noexcept -> behaviour & = delete;
		virtual ~behaviour(void) noexcept override = default;

		inline virtual auto name(void) noexcept -> char const* const = 0;

		virtual void start(void) noexcept {};
		virtual void update(void) noexcept = 0;
	};
}