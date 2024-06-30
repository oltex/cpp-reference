#include "sct.h"

namespace window {
	sct::sct(void) noexcept {
		initialize();
	}
	void sct::initialize(void) noexcept {
		memset(&_wsex, 0, sizeof(WNDSTRUCTEXW));
	}
}