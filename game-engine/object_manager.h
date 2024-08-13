#pragma once
#include "design-pattern/singleton.h"
#include "data-structure/list.h"
#include "data-structure/map.h"

#include "object.h"
#include <string>

class object_manager final : public singleton<object_manager> {
	friend class singleton<object_manager>;
private:
	inline explicit object_manager(void) noexcept = default;
	inline ~object_manager(void) noexcept {
	};
public:
	inline void update(void) noexcept {
		for (auto iter = _object.begin(); iter != _object.end();) {
			if (true == (*iter)->_destory) {

			}
			else {

			}
		}
	}
public:
	inline void create(std::string const key) noexcept {

	}
	inline auto clone() noexcept -> object* {
		object* prototype = _prototype.find();
	}
private:
	map<std::string, object* const> _prototype;
	list<object* const> _object;
};
