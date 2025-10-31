//#pragma once
//#include "library/container/pointer.h"
//#ifdef _DEBUG
//#pragma comment(lib, "library/fmod/fmodL_vc.lib")
//#else
//#pragma comment(lib, "library/fmod/fmod_vc.lib")
//#endif
//#include "library/fmod/fmod.hpp"
//#include "resource.h"
//
//namespace framework {
//	class sound : public resourcer<sound, "sound"> {
//		inline static void sound_deleter(FMOD::Sound* sound) noexcept {
//			sound->release();
//		}
//		library::share_pointer<FMOD::Sound> _sound;
//	public:
//		explicit sound(library::string_view path, FMOD_MODE const mode) noexcept;
//		explicit sound(sound const& rhs) noexcept;
//		explicit sound(sound&&) noexcept = delete;
//		auto operator=(sound const&) noexcept -> sound & = delete;
//		auto operator=(sound&&) noexcept -> sound & = delete;
//		virtual ~sound(void) noexcept override;
//	};
//}