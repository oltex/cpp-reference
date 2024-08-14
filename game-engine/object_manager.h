#pragma once
#include "design-pattern/singleton.h"
#include "data-structure/list.h"
#include "data-structure/unordered_map.h"

#include "object.h"
#include <string>

namespace engine {
	class object_manager final : public singleton<object_manager> {
		friend class singleton<object_manager>;
	private:
		inline explicit object_manager(void) noexcept = default;
		inline ~object_manager(void) noexcept {
		};
		inline explicit object_manager(object_manager const& rhs) noexcept = delete;
		inline auto operator=(object_manager const& rhs) noexcept -> object_manager & = delete;
		inline explicit object_manager(object_manager&& rhs) noexcept = delete;
		inline auto operator=(object_manager&& rhs) noexcept -> object_manager & = delete;
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
		inline void create(std::string const key, object* const prototype) noexcept {
			_prototype.emplace(key, prototype);
		}
		inline auto clone(std::string const key) noexcept -> object* {
			object* prototype = (*_prototype.find(key))._second;
			object* clone = prototype->clone();
			_object.emplace_back(clone);
			return clone;
		}
	private:
		unordered_map<std::string, object* const> _prototype;
		list<object* const> _object;
	};
}