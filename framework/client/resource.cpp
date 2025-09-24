#include "resource.h"
#include "sound.h"

namespace framework {
	void resources::load_sound(char const* const name, char const* const path) noexcept {
		auto resource = sound(path, FMOD_DEFAULT);
	}
}
