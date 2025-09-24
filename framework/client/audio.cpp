#include "audio.h"

namespace framework {
	audio::audio(void) noexcept {
		FMOD::System_Create(&_system);
		_system->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);
	}
	audio::~audio(void) noexcept {
		_system->close();
		_system->release();
	}
	void audio::update_sound(void) noexcept {
		_system->update();
	}
}