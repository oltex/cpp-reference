#pragma once
#include "window.h"
#include "graphic.h"
#include "frame.h"

#include "object_manager.h"
#include "component_manager.h"
#include "scene_manager.h"

#include "transform.h"
#include "camera.h"

namespace framework {
	class client : public library::singleton<client> {
		friend class library::singleton<client>;
		window _window;
		graphic _graphic;
		frame _frame;
	protected:
		object_manager _object_manager;
		component_manager _component_manager;
		scene_manager _scene_manager;
	public:
		explicit client(void) noexcept;
		inline explicit client(client const&) noexcept = delete;
		inline explicit client(client&&) noexcept = default;
		inline auto operator=(client const&) noexcept -> client & = delete;
		inline auto operator=(client&&) noexcept -> client & = default;
		inline ~client(void) noexcept = default;

		void execute(void) noexcept;
		auto create_scene(library::string const& path) noexcept;

		void regist_object(library::string const& name, object_share_ptr& object) noexcept;
	};
}