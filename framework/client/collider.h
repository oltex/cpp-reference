#pragma once
#include "component.h"
#pragma comment(lib, "module/directx/binary/directx.lib")
#include "module/directx/directx.h"

namespace framework {
	class collider : public component, public componentr<collider, "collider"> {
		dcoll::bound _bound;
	public:
		explicit collider(void) noexcept;
		explicit collider(collider const&) noexcept = delete;
		explicit collider(collider&&) noexcept = delete;
		auto operator=(collider const&) noexcept -> collider & = delete;
		auto operator=(collider&&) noexcept -> collider & = delete;
		virtual ~collider(void) noexcept override = default;
	};
}