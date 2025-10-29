#pragma once
#include "resource.h"

namespace framework {
	class level : public resourcer<level, "level"> {
		using base = resourcer<level, "level">;
		nlohmann::json _data;
	public:
		explicit level(library::string_view name) noexcept;
		explicit level(nlohmann::json const& json) noexcept;
		explicit level(level const&) noexcept = delete;
		explicit level(level&&) noexcept = delete;
		auto operator=(level const&) noexcept -> level & = delete;
		auto operator=(level&&) noexcept -> level & = delete;
		virtual ~level(void) noexcept override = default;

		virtual void save(nlohmann::json& json) noexcept override;
		virtual void open(void) noexcept override;
	};
}