#pragma once
#ifdef _DEBUG
#pragma comment(lib, "fmod/fmodL.lib") 
#else
#pragma comment(lib, "fmod/fmod.lib")
#endif
#include "design-pattern/singleton.h"
#include "fmod/fmod.hpp"

namespace engine {
	class sound_manager final : public design_pattern::singleton<sound_manager> {
		friend class design_pattern::singleton<sound_manager>;
	//public:
	//	enum class EChannel : _ubyte { BGM, CH0, CH1, CH2, CH3, END };
	private:
		inline explicit sound_manager(void) noexcept {
			FMOD::System_Create(&_system);
			_system->init(FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, nullptr);
			_system->set3DSettings(1.f, 0.1f, 1.f);
		};
		inline explicit sound_manager(sound_manager const& rhs) noexcept = delete;
		inline auto operator=(sound_manager const& rhs) noexcept -> sound_manager & = delete;
		inline explicit sound_manager(sound_manager&& rhs) noexcept = delete;
		inline auto operator=(sound_manager&& rhs) noexcept -> sound_manager & = delete;
		inline ~sound_manager(void) override {
			_system->close();
			_system->release();
			
		};
	public:
		inline void update(void) noexcept {
			_system->update();
		}
	private:
		FMOD::System* _system;
	};
}