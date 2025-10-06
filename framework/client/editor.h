#pragma once
#include "library/pattern/singleton.h"
#include "library/container/vector.h"
#include "library/container/pointer.h"

namespace framework {
	class editor {
	public:
		explicit editor(void) noexcept = default;
		explicit editor(editor const&) noexcept = delete;
		explicit editor(editor&&) noexcept = delete;
		auto operator=(editor const&) noexcept -> editor & = delete;
		auto operator=(editor&&) noexcept -> editor & = delete;
		virtual ~editor(void) noexcept = default;

		virtual void update(void) noexcept = 0;
	};
	class editors : public library::singleton<editors> {
		friend class library::singleton<editors>;

		library::vector<library::unique_pointer<editor>> _editor;
		explicit editors(void) noexcept;
		explicit editors(editors const&) noexcept = delete;
		explicit editors(editors&&) noexcept = delete;
		auto operator=(editors const&) noexcept -> editors & = delete;
		auto operator=(editors&&) noexcept -> editors & = delete;
		~editors(void) noexcept;
	public:
		void update(void) noexcept;
		void render(void) noexcept;
	};
}