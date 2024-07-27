#pragma once
#include "singleton.h"
#include <memory>
#include <intrin.h>
#include <cstdint>

#include <iostream>

class cache_simulator final : public singleton<cache_simulator> {
	friend class singleton<cache_simulator>;
private:
	struct cache_line {
		size_t _tag;
	};
	class cache_way final {
	public:
		inline explicit cache_way(size_t const size, unsigned long tag) noexcept
			: _size(size), _shift(tag) {
			_line = (cache_line*)malloc(sizeof(cache_line) * size);
			memset(_line, 1, sizeof(cache_line) * size);
		}
		inline ~cache_way(void) noexcept {
			free(_line);
		}
	public:
		inline void access(void const* const ptr) noexcept {
			auto tag = (size_t)ptr >> _shift;
			for (auto i = 0; i < _size; ++i)
				if (_line[i]._tag == tag) {
					select(i);
					std::cout << "캐시히트" << std::endl;
					return;
				}
			auto idx = replace();
			_line[idx]._tag = tag;

			std::cout << "캐시미스" << std::endl;
		}
	private:
		inline void select(size_t const idx) noexcept {
			auto parent = (idx - (idx % 2 ? 1 : 0) + _size) / 2;
			auto child = idx;
			while (parent > 0) {
				size_t bit = 1 << (_size - parent - 1);
				if (child % 2)
					_plru &= ~bit;
				else
					_plru |= bit;

				child = parent;
				parent = (child) / 2;
			}
		}
		inline auto replace(void) noexcept -> size_t {
			size_t parent = 1;

			while (parent < _size) {
				size_t bit = 1 << (_size - parent - 1);
				if (_plru & bit)
					_plru &= ~bit;
				else
					_plru |= bit;
				parent = parent * 2 + ( _plru & bit ? 0 : 1);
			}

			return parent - _size;
		}
	private:
		cache_line* _line;
		size_t _size;

		unsigned long _shift;
		unsigned short _plru = 0;
	};
	class cache_set final {
	public:
		inline explicit cache_set(size_t const set, size_t const way, size_t mask,
			unsigned long idx, unsigned long tag) noexcept
			: _size(set), _mask(mask), _shift(idx) {
			_way = (cache_way*)malloc(sizeof(cache_way) * set);
			for (auto i = 0; i < set; ++i)
				new(_way + i) cache_way(way, tag);
		}
		inline ~cache_set(void) noexcept {
			for (auto i = 0; i < _size; ++i)
				_way[i].~cache_way();
			free(_way);
		}
	public:
		inline void access(void const* const ptr) const noexcept {
			auto idx = ((size_t)ptr & _mask) >> _shift;
			_way[idx].access(ptr);
		}
	private:
		cache_way* _way;
		size_t _size;

		size_t _mask;
		unsigned long _shift;
	};
private:
	inline explicit cache_simulator(void) noexcept {
		int reg[4];
		__cpuidex(reg, 0x00000004, 0x00000000);

		size_t set = reg[2] + 1;
		size_t way = ((reg[1] & 0xffc00000) >> 22) + 1;
		size_t line = (reg[1] & 0xfff) + 1;

		unsigned long idx;
		_BitScanReverse(&idx, line);
		size_t mask = (set - 1) << idx;

		unsigned long tag;
		_BitScanReverse(&tag, set);
		tag += idx;

		_set = new cache_set(set, way, mask, idx, tag);
	}
	inline ~cache_simulator(void) noexcept {
		delete _set;
	}
public:
	inline void access(void* a) {
		_set->access(a);
	}
public:
	cache_set* _set;
};

// (((reg.ebx & 0xffc00000) >> 22) + 1) * (((reg.ebx & 0x3ff000) >> 12) + 1) * ((reg.ebx & 0xfff) + 1) * (reg.ecx + 1);
// (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)
// (Ways + 1) * (Partitions + 1) * (Line_Size + 1)* (Sets + 1)