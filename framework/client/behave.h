#pragma once
#include "system.h"
#include "behaviour.h"

namespace framework {
	class behave : public system {
		library::list<library::intrusive::weak_pointer<behaviour, 0>> _behaviour;
	public:
		explicit behave(void) noexcept = default;
		explicit behave(behave const&) noexcept = delete;
		explicit behave(behave&&) noexcept = delete;
		auto operator=(behave const&) noexcept -> behave & = delete;
		auto operator=(behave&&) noexcept -> behave & = delete;
		virtual ~behave(void) noexcept override = default;

		virtual void update(void) noexcept override;
		virtual void add_component(library::string const& key, library::vector<library::intrusive::share_pointer<component, 0>>  const& component) noexcept override;
	};
}