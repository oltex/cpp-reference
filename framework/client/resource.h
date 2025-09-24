#pragma once
#include "library/container/hash_table.h"
#include "library/pattern/singleton.h"

namespace framework {
	class resource {
	public:
		explicit resource(void) noexcept = default;
		explicit resource(resource const&) noexcept = delete;
		explicit resource(resource&&) noexcept = delete;
		auto operator=(resource const&) noexcept -> resource & = delete;
		auto operator=(resource&&) noexcept -> resource & = delete;
		virtual ~resource(void) noexcept = default;
	};

	class resources : library::singleton<resources> {
		friend class library::singleton<resources>;

		explicit resources(void) noexcept = default;
		explicit resources(resources const&) noexcept = delete;
		explicit resources(resources&&) noexcept = delete;
		auto operator=(resources const&) noexcept -> resources & = delete;
		auto operator=(resources&&) noexcept -> resources & = delete;
		~resources(void) noexcept = default;
	public:
		void load_sound(char const* const name, char const* const path) noexcept;
	};
}