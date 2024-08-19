#pragma once


//WASAPI ASIO중에 하나 쓰기? 더있나 찾아보기
namespace engine {
	class sound_manager final {
	public:
		enum class EChannel : _ubyte { BGM, CH0, CH1, CH2, CH3, END };
	private:
		inline explicit sound_manager(void) noexcept {
			FMOD_System_Create(&m_pSystem, FMOD_VERSION);
			FMOD_System_Init(m_pSystem, FMOD_MAX_CHANNEL_WIDTH, FMOD_INIT_NORMAL, NULL);
			FMOD_System_Set3DSettings(m_pSystem, 1.f, 0.1f, 1.f);
		};
		virtual ~CSound(void) override = default;
		virtual void Delete(void) override;
	public:
		const HRESULT Init(const _ubyte& byScene);
		void Update(void);
	public:
		const HRESULT Load_Sound(const _ubyte& byScene, const _tchar* const& pFilePath);
	public:
		FMOD_SYSTEM* const Get_System(void);
		FMOD_SOUND* const Get_Sound(const _ubyte& byScene, const _tchar* const& pSoundKey);
		const _byte Get_Scene(void);

		const HRESULT Clear_Sound(const _ubyte& byScene) const;
	protected:
		FMOD_SOUND* const Find_Sound(const _ubyte& byScene, const _tchar* const& pSoundKey) const;
	private:
		FMOD_SYSTEM* m_pSystem = nullptr;
		map<wstring, FMOD_SOUND*>* m_pMapSound;
	private:
		_ubyte m_byScene = 0;

		FMOD_CHANNEL* m_pChannel[static_cast<_ubyte>(EChannel::END)];
		EChannel m_eChannel = EChannel::CH0;
	};
}