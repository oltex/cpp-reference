#pragma once
#include "design-pattern/singleton.h"

class engine final : public singleton<engine> {
	friend class singleton<engine>;
private:
	inline explicit engine(void) noexcept {

	};
	inline ~engine(void) noexcept = default;
public:
	inline void update(void) const noexcept {

	};
private:
	graphic_mgr& graph_mgr;
	timer_mgr& _timer_mgr;
	key_mgr& _key_mgr;
	component_mgr& _component_mgr;
	object_mgr& _object_mgr;
	scene_mgr& _scene_mgr;
	fps _fps;
};