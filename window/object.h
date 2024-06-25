#pragma once
#include <Windows.h>

namespace window {
	class object {
		friend class dc;
	public:
		explicit object(void) noexcept = default;
		virtual ~object(void) noexcept;
	protected:
		HGDIOBJ _hgdiobj = nullptr;
	};
}

