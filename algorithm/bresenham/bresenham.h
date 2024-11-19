#pragma once
#include "../../data-structure/list/list.h"

class bresenham final {
private:
	using size_type = unsigned int;
public:
	struct coordinate final {
	public:
		inline bool operator ==(coordinate const& rhs) const noexcept {
			return (_x == rhs._x && _y == rhs._y);
		}
		inline bool operator !=(coordinate const& rhs) const noexcept {
			return (_x != rhs._x && _y != rhs._y);
		}
		size_type _x, _y;
	};
public:
	inline auto calculate(coordinate source, coordinate destination) noexcept -> data_structure::list<coordinate> {
		coordinate size{ abs(static_cast<int>(source._x) - static_cast<int>(destination._x)), abs(static_cast<int>(source._y) - static_cast<int>(destination._y)) };
		coordinate step{ source._x < destination._x ? 1 : -1, source._y < destination._y ? 1 : -1 };
		size_type error = (size._x > size._y ? size._x : size._y) / 2;
		data_structure::list<coordinate> result;

		if (size._x >= size._y) {
			for (;;) {
				result.emplace_back(source);
				if (source == destination)
					break;
				source._x += step._x;
				error += size._y;
				if (size._x <= error) {
					source._y += step._y;
					error -= size._x;
				}
			}
		}
		else {
			for (;;) {
				result.emplace_back(source);
				if (source == destination)
					break;
				source._y += step._y;
				error += size._x;
				if (size._y <= error) {
					source._x += step._x;
					error -= size._y;
				}
			}
		}
		return result;
	};
};