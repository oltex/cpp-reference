#include "sound.h"

namespace framework {
	sound::sound(void) noexcept {
		FMOD::System_Create(&_system);
		_system->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);

	}
	sound::~sound(void) noexcept {
		_system->close();
		_system->release();
	}
}