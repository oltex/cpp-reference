#pragma once
#include "window.h"
#include "graphic.h"
#include "editor.h"
#include "timer.h"
#include "input.h"
#include "audio.h"

#include "resource.h"
#include "component.h"
#include "object.h"
#include "scene.h"

namespace framework {
	class client : public library::singleton<client> {
		friend class library::singleton<client>;
		window& _window;
		graphic& _graphic;
		editor& _editor;
		input& _input;
		audio& _audio;
		timer _timer;

		resources& _resources;
		components& _components;
		objects& _objects;
		scenes& _scenes;
	public:
		explicit client(void) noexcept;
		explicit client(client const&) noexcept = delete;
		explicit client(client&&) noexcept = default;
		auto operator=(client const&) noexcept -> client & = delete;
		auto operator=(client&&) noexcept -> client & = default;
		~client(void) noexcept;

		void execute(void) noexcept;
	};
}