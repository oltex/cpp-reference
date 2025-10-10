#pragma once
#include "behaviour.h"

namespace framework {
	class camera_move : public behaviour {
	public:
		explicit camera_move(library::intrusive::share_pointer<object, 0>& parent) noexcept;
		explicit camera_move(camera_move const&) noexcept = delete;
		explicit camera_move(camera_move&&) noexcept = delete;
		auto operator=(camera_move const&) noexcept -> camera_move & = delete;
		auto operator=(camera_move&&) noexcept -> camera_move & = delete;
		virtual ~camera_move(void) noexcept override = default;

		virtual void update(void) noexcept override;
	};
}