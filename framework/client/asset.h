#pragma once
#include "editor.h"
#include "library/system/guid.h"
#include <vector>

namespace framework {
	class asset : public editor {
		enum column{
			name,
			type
		};
		struct item {
			library::guid _guid;
			std::string _name;
			std::string _type;
		};
		std::vector<item> _item;
	public:
		explicit asset(void) noexcept {
			_item.emplace_back(library::guid(), "aaa", "kkk");
			_item.emplace_back(library::guid(), "bbb", "ggg");
			_item.emplace_back(library::guid(), "ccc", "kkk");
			_item.emplace_back(library::guid(), "ddd", "ggg");
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