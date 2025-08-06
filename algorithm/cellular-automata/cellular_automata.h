#pragma once
#include "data-structure/bit_grid.h"
#include <iostream>

class cellular_automata final {
private:
	using size_type = unsigned int;
public:
	inline explicit cellular_automata(size_type const width, size_type height) noexcept
		: _bit_grid(width, height) {
	}
	//not implemented
	inline explicit cellular_automata(cellular_automata const& rhs) noexcept;
	//not implemented
	inline explicit cellular_automata(cellular_automata&& rhs) noexcept;
	//not implemented
	inline auto operator=(cellular_automata const& rhs) noexcept -> cellular_automata&;
	//not implemented
	inline auto operator=(cellular_automata&& rhs) noexcept -> cellular_automata&;
	inline ~cellular_automata(void) noexcept = default;
public:
	inline void initalize(float const wall_probability) noexcept {
		_bit_grid.clear();
		size_type width = _bit_grid.get_width();
		size_type height = _bit_grid.get_height();

		for (size_type y = 0; y < height; ++y)
			for (size_type x = 0; x < width; ++x)
				_bit_grid.set_bit(x, y, wall_probability > std::rand() / (float)RAND_MAX ? true : false);
	}
	inline void simulate(size_type const repeat) noexcept {
		size_type width = _bit_grid.get_width();
		size_type height = _bit_grid.get_height();
		for (size_type r = 0; r < repeat; ++r) {
			for (size_type y = 0; y < height; ++y) {
				for (size_type x = 0; x < width; ++x) {
					unsigned char count = 0;
					for (unsigned char i = 0; i < 8; ++i) {
						static char constexpr offset[][2] = { {0, -1}, {1, -1}, {1, 0}, {1, 1}, {0, 1}, {-1, 1}, {-1, 0}, {-1, -1} };
						size_type nx = x + offset[i][0];
						size_type ny = y + offset[i][1];
						if (nx >= 0 && ny >= 0 && nx < width && ny < height)
							count += _bit_grid.get_bit(nx, ny);
						else
							count++;
					}

					if (4 < count)
						_bit_grid.set_bit(x, y, true);
					else if (4 > count)
						_bit_grid.set_bit(x, y, false);
				}
			}
		}
	}
public:
	inline void print(void) const noexcept {
		size_type width = _bit_grid.get_width();
		size_type height = _bit_grid.get_height();
		for (size_type y = 0; y < height; ++y) {
			for (size_type x = 0; x < width; ++x)
				std::cout << _bit_grid.get_bit(x, y);
			std::cout << std::endl;
		}
	}
private:
	library::data_structure::bit_grid<long long> _bit_grid;
};