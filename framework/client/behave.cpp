#include "behave.h"

namespace framework {
	void behave::update(void) noexcept {
		for (auto& behaviour : _behaviour) {
			behaviour.lock()->update();
		}
	}
	void behave::add_component(library::string const& key, library::vector<library::intrusive::share_pointer<component, 0>> const& component) noexcept {
		_behaviour.emplace_back(component[0]);
	}
}
