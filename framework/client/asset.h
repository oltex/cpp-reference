#pragma once
#include "library/system/guid.h"
#include "editor.h"
#include "resource.h"
#include <vector>

namespace framework {
	class asset : public editor {
		enum column {
			name,
			type
		};
		struct item {
			library::rcu_pointer<resource> _pointer;
			std::string _name;
			std::string _type;
		};
		std::vector<item> _item;
	public:
		explicit asset(void) noexcept {
		}
		explicit asset(asset const&) noexcept = delete;
		explicit asset(asset&&) noexcept = delete;
		auto operator=(asset const&) noexcept -> asset & = delete;
		auto operator=(asset&&) noexcept -> asset & = delete;
		virtual ~asset(void) noexcept override = default;

		virtual void update(void) noexcept override;

		void import_file(void) noexcept;
		void search(void) noexcept;
	};
}