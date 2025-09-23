#pragma once
#include "window.h"
#include "graphic.h"
#include "timer.h"

//#include "object_manager.h"
//#include "component_manager.h"
//#include "scene_manager.h"
//#include "transform.h"
//#include "camera.h"
namespace framework {
	class client : public library::singleton<client> {
		friend class library::singleton<client>;
		window _window;
		graphic _graphic;
		framework::timer _timer;
	protected:
		//object_manager _object_manager;
		//scene_manager _scene_manager;
	public:
		explicit client(void) noexcept;
		explicit client(client const&) noexcept = delete;
		explicit client(client&&) noexcept = default;
		auto operator=(client const&) noexcept -> client & = delete;
		auto operator=(client&&) noexcept -> client & = default;
		~client(void) noexcept = default;

		void execute(void) noexcept;
		//auto create_scene(library::string const& path) noexcept;
	};
}