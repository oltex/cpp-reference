#pragma once
#include "scene.h"

class my_scene : public framework::scene {
public:
	inline explicit my_scene(void) noexcept = default;
	inline explicit my_scene(my_scene const&) noexcept = delete;
	inline explicit my_scene(my_scene&&) noexcept = delete;
	inline auto operator=(my_scene const&) noexcept -> my_scene & = delete;
	inline auto operator=(my_scene&&) noexcept -> my_scene & = delete;
	inline virtual ~my_scene(void) noexcept = default;

	inline virtual void update(void) noexcept {
	};
};