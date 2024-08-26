#pragma once
#include <GameInput.h>

namespace engine {
	class keyboard final {
		inline explicit device(IGameInputDevice* device_, unsigned int const type) noexcept
			: _device(device_), _type(type) {
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
			auto info = _device->GetDeviceInfo();
			input.GetCurrentReading(info->supportedInput, _device, &_reading);
			input.GetPreviousReading(_reading, info->supportedInput, _device, &_prev_reading);
		}
	private:
		IGameInputReading* _reading;
		IGameInputReading* _prev_reading;
	};
}