#pragma once
#include <Windows.h>

namespace timer {
	class high_resolution {

	private:
		inline static LARGE_INTEGER _frequency;
	};
}