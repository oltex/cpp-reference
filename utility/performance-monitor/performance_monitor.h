#pragma once
#include "../../design-pettern/singleton/singleton.h"
#include <Pdh.h>
#pragma comment(lib,"pdh.lib")

class performance_monitor final : public design_pattern::singleton<performance_monitor> {
private:
	friend class design_pattern::singleton<performance_monitor>;
private:
	inline explicit performance_monitor(void) noexcept {
		PdhOpenQueryW(nullptr, NULL, &_qurey);
	};
	inline explicit performance_monitor(performance_monitor const& rhs) noexcept = delete;
	inline explicit performance_monitor(performance_monitor&& rhs) noexcept = delete;
	inline auto operator=(performance_monitor const& rhs) noexcept -> performance_monitor & = delete;
	inline auto operator=(performance_monitor&& rhs) noexcept -> performance_monitor & = delete;
	inline ~performance_monitor(void) noexcept {
		//PdhRemoveCounter
		PdhCloseQuery(&_qurey);
	}
private:
	PDH_HQUERY _qurey;
};