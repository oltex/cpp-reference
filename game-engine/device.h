#pragma once
#include <GameInput.h>

namespace engine {
	class device final {
	public:
		inline explicit device(IGameInputDevice* device_, unsigned int const type) noexcept
			: _device(device_), _type(type) {
			auto info = _device->GetDeviceInfo();
			_device.
			int a = 10;
			GameInputKind supportedInput
		};
		inline explicit device(device const& rhs) = delete;
		inline auto operator=(device const& rhs) noexcept -> device & = delete;
		inline explicit device(device&& rhs) noexcept = delete;
		inline auto operator=(device&& rhs) noexcept -> device & = delete;
		inline virtual ~device(void) noexcept {
			_device->Release();
		};
	public:
		inline void update(IGameInput& input) noexcept {
		}
	private:
		unsigned int _type;
		IGameInputDevice* _device;
	};
}