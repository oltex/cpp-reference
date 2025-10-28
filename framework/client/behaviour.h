#pragma once
#include "component.h"
#include "object.h"

namespace framework {
	class behaviour : public component {
	protected:
		library::rcu_pointer<object> _parent;
	public:
		explicit behaviour(library::rcu_pointer<object> parent) noexcept;
		explicit behaviour(behaviour const&) noexcept = delete;
		explicit behaviour(behaviour&&) noexcept = delete;
		auto operator=(behaviour const&) noexcept -> behaviour & = delete;
		auto operator=(behaviour&&) noexcept -> behaviour & = delete;
		virtual ~behaviour(void) noexcept override = default;

		virtual auto type(void) noexcept -> char const* const = 0;

		virtual void start(void) noexcept {};
		virtual void update(void) noexcept = 0;
	};
}