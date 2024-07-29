#pragma once
#include "singleton.h"
#include <memory>
#include <intrin.h>
#include <cstdint>

#include <iostream>



class cache_simulator final : public singleton<cache_simulator> {
	friend class singleton<cache_simulator>;
private:
	using cache_line = size_t;
	class cache_way {
	public:
		inline explicit cache_way(size_t const size, unsigned long const tag) noexcept
			: _size(size), _shift(tag) {
			_line = (cache_line*)malloc(sizeof(cache_line) * size);
			memset(_line, 1, sizeof(cache_line) * size);
		}
		inline virtual ~cache_way(void) noexcept {
			free(_line);
		}
	public:
		inline virtual void access(void const* const ptr) noexcept = 0;
	protected:
		cache_line* _line;
		size_t const _size;
		unsigned long const _shift;
	};
	class tree_plur final : public cache_way {
	public:
		using cache_way::cache_way;
	public:
		inline virtual void access(void const* const ptr) noexcept override {
			auto tag = (size_t)ptr >> _shift;
			for (size_t i = 0; i < _size; ++i)
				if (_line[i] == tag) {
					select(i);
					std::cout << "캐시히트" << std::endl;
					return;
				}
			auto idx = replace();
			_line[idx] = tag;
			std::cout << "캐시미스" << std::endl;
		}
	private:
		inline void select(size_t const idx) noexcept {
			auto parent = (idx - (idx % 2 ? 1 : 0) + _size) / 2;
			auto child = idx;
			while (parent > 0) {
				unsigned short bit = 1 << (_size - parent - 1);
				if (child % 2)
					_bit &= ~bit;
				else
					_bit |= bit;

				child = parent;
				parent = (child) / 2;
			}
		}
		inline auto replace(void) noexcept -> size_t {
			size_t parent = 1;

			while (parent < _size) {
				unsigned short bit = 1 << (_size - parent - 1);
				if (_bit & bit)
					_bit &= ~bit;
				else
					_bit |= bit;
				parent = parent * 2 + (_bit & bit ? 0 : 1);
			}

			return parent - _size;
		}
	private:
		unsigned short _bit = 0;
	};
	class cache_set final {
		friend class cache_simulator;
	public:
		inline explicit cache_set(size_t const set, size_t mask, unsigned long line) noexcept
			: _size(set), _mask(mask), _shift(line) {
		}
		inline ~cache_set(void) noexcept {
			for (size_t i = 0; i < _size; ++i)
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
		size_t const _size;
		size_t const _mask;
		unsigned long const _shift;
	};
private:
	inline explicit cache_simulator(void) noexcept {
		int reg[4];
		__cpuidex(reg, 0x00000004, 0x00000000);

		size_t set = reg[2] + 1;
		unsigned long line = (reg[1] & 0xfff) + 1;
		_BitScanReverse(&line, line);
		size_t mask = (set - 1) << line;
		_set = new cache_set(set, mask, line);

		size_t way = ((reg[1] & 0xffc00000) >> 22) + 1;
		unsigned long tag;
		_BitScanReverse(&tag, set);
		tag += line;
		_set->_way = (tree_plur*)malloc(sizeof(tree_plur) * set);
		for (size_t i = 0; i < set; ++i)
			new(_set->_way + i) tree_plur(way, tag);
	}
	inline ~cache_simulator(void) noexcept {
		delete _set;
	}
public:
	inline void access(void const* const ptr) const noexcept {
		_set->access(ptr);
	}
public:
	cache_set* _set;
};

// (((reg.ebx & 0xffc00000) >> 22) + 1) * (((reg.ebx & 0x3ff000) >> 12) + 1) * ((reg.ebx & 0xfff) + 1) * (reg.ecx + 1);
// (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)
// (Ways + 1) * (Partitions + 1) * (Line_Size + 1)* (Sets + 1)