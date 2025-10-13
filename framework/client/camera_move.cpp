#include "camera_move.h"
#include "transform.h"
#include "input.h"

namespace framework {
	camera_move::camera_move(library::intrusive::share_pointer<object, 0>& parent) noexcept
		: behaviour(component::type_id<camera_move>(), parent) {
	}

	void camera_move::update(void) noexcept {
		auto& input = input::instance();
		auto parent = _parent.lock();

		transform& transform = static_cast<framework::transform&>(parent->_component.front());

		if (key::w & input.get_keyboard_button()) 
			transform.translate(dmath::float3(0.f, 0.f, 0.01f));
		if (key::s & input.get_keyboard_button()) 
			transform.translate(dmath::float3(0.f, 0.f, -0.01f));
		if (key::a & input.get_keyboard_button()) 
			transform.translate(dmath::float3(-0.01f, 0.f, 0.f));
		if (key::d & input.get_keyboard_button()) 
			transform.translate(dmath::float3(0.01f, 0.f, 0.f));
		//transform.rotate(dmath::float3(input.get_mouse_move()._second / 1000.f, input.get_mouse_move()._first / 1000.f, 0.f));
		transform.rotate(dmath::float3(input.get_mouse_move()._second / 1000.f, 0.f, 0.f));
		transform.rotate(dmath::float3(0.f, input.get_mouse_move()._first / 1000.f, 0.f), false);
		//parent.get
	}
}
