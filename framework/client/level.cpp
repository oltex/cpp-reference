#include "level.h"
#include "scene.h"

namespace framework {
	level::level(library::string_view name) noexcept 
		: base(name) {
		_data["x"] = 10;
	}
	level::level(nlohmann::json const& json) noexcept 
		: base(json) {
		_data = json["data"];

	}

	void level::save(nlohmann::json& json) noexcept {
		resource::save(json);
		json["data"] = _data;
	}
	void level::open(void) noexcept {
		//scenes::instance().open(*this);
	}
}
