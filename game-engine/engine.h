#pragma once
#include "design-pattern/singleton.h"

namespace engine {
	class engine final : public singleton<engine> {
		friend class singleton<engine>;
	private:
		inline explicit engine(void) noexcept {

		};
		inline ~engine(void) noexcept = default;
	public:
		inline void update(void) const noexcept {

		};
	private:
	};
}