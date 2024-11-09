#pragma once
#include "library/window/window.h"
#include "library/data-structure/map.h"
#include <map>

class test final {
public:
	inline void onoff(window::window& window) noexcept {
		if (false == _timer) {
			window.set_timer(1, 0, nullptr);
			_timer = true;
		}
		else {
			window.kill_timer(1);
			_timer = false;
		}
	}
	inline void run(data_structure::map<int,int>& map) noexcept {
		auto value = rand() % 40;
		if (false == rand() % 2) {
			map.emplace(value, 0);
			_map.emplace(value, 0);
		}
		else {
			map.erase(value);
			_map.erase(value);

		}
		auto j = _map.begin();
		for (auto i = map.begin(); i != map.end(); ++i, ++j) {
			if ((*i)._first != (*j).first)
				break;
		}
		map.debug(map._root);
	}
private:
	bool _timer = false;

	std::map<int, int> _map;

};