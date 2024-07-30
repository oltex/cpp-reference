#pragma once
#include "singleton.h"
#include "vector.h"
#include "unordered_map.h"

#include <intrin.h>
#include <cstdint>
#include <string>

#define cache_simulator_access(identifier) \
cache_simulator::instance().access(__FILE__, __LINE__, #identifier, (void*)&identifier)

class cache_simulator final : public singleton<cache_simulator> {
	friend class singleton<cache_simulator>;
	struct cache_key final {
		char const* const _file;
		int const _line;
		inline bool operator==(cache_key const& rhs) noexcept {
			if (_file != rhs._file || _line != rhs._line)
				return false;
			return true;
		}
	};
	struct cache_value final {
		char const* const _identifier;
		size_t _hit = 0;
		size_t _miss = 0;
	};
private:
	class cache_way {
	public:
		inline explicit cache_way(void) noexcept = default;
		inline virtual ~cache_way(void) noexcept = default;
	public:
		inline virtual bool access(void const* const ptr) noexcept = 0;
	};
	class tree_plur final : public cache_way {
		using cache_line = uintptr_t;
	public:
		inline explicit tree_plur(size_t const size, unsigned long const shift) noexcept
			: _shift(shift) {
			_line.assign(size, 0xffffff);
		}
		inline virtual ~tree_plur(void) noexcept = default;
	public:
		inline virtual bool access(void const* const address) noexcept override {
			uintptr_t tag = (uintptr_t)address >> _shift;
			for (size_t i = 0; i < _line.size(); ++i)
				if (_line[i] == tag) {
					select(i);
					return true;
				}
			auto idx = replace();
			_line[idx] = tag;
			return false;
		}
	private:
		inline void select(size_t const idx) noexcept {
			auto size = _line.size();
			auto parent = (idx - (idx % 2 ? 1 : 0) + size) / 2;
			auto child = idx;

			while (parent > 0) {
				unsigned short bit = 1 << (size - parent - 1);
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
			auto size = _line.size();

			while (parent < size) {
				unsigned short bit = 1 << (size - parent - 1);
				if (_bit & bit)
					_bit &= ~bit;
				else
					_bit |= bit;
				parent = parent * 2 + (_bit & bit ? 0 : 1);
			}

			return parent - size;
		}
	private:
		vector<cache_line> _line;
		unsigned long const _shift;
		unsigned short _bit = 0;
	};
	class cache_set final {
		friend class cache_simulator;
	public:
		inline explicit cache_set(size_t const size, size_t mask, unsigned long shift) noexcept
			:_mask(mask), _shift(shift) {
			_way.reserve(size);
		}
		inline ~cache_set(void) noexcept {
			for (auto& iter : _way)
				delete iter;
		}
	public:
		inline bool access(void const* const address) const noexcept {
			auto idx = ((size_t)address & _mask) >> _shift;
			return _way[idx]->access(address);
		}
	private:
		vector<cache_way*> _way;
		size_t const _mask;
		unsigned long const _shift;
	};
private:
	inline explicit cache_simulator(void) noexcept {
		int reg[4];
		__cpuidex(reg, 0x00000004, 0x00000000);

		size_t set = reg[2] + 1;
		size_t way = ((reg[1] & 0xffc00000) >> 22) + 1;
		unsigned long line = (reg[1] & 0xfff) + 1;
		_BitScanReverse(&line, line);
		size_t mask = (set - 1) << line;
		unsigned long tag;
		_BitScanReverse(&tag, set);
		tag += line;

		_set = new cache_set(set, mask, line);
		for (size_t i = 0; i < set; ++i)
			_set->_way.emplace_back(new tree_plur(way, tag));
	}
	inline ~cache_simulator(void) noexcept {
		delete _set;
	}
public:
	inline void access(char const* const file, int const line, char const* const identifier, void const* const address) noexcept {
		cache_key key{ file, line };

		auto iter = _result.find(key);
		if (iter == _result.end())
			iter = _result.emplace(key, cache_value{ identifier });

		auto& value = (*iter)._second;
		if (_set->access(address))
			++value._hit;
		else
			++value._miss;
	}
public:
	void print(void) const noexcept {
		printf("file | line | identifier | hit | miss\n");
		printf("---------------------------\n");
		for (auto& iter : _result) {
			auto& key = iter._first;
			auto& value = iter._second;
			printf("%s | %d | %s | %llu | %llu\n",
				key._file, key._line,
				value._identifier, value._hit, value._miss);
		}
	}
private:
	cache_set* _set;
	unordered_map<cache_key, cache_value> _result;
};

// (((reg.ebx & 0xffc00000) >> 22) + 1) * (((reg.ebx & 0x3ff000) >> 12) + 1) * ((reg.ebx & 0xfff) + 1) * (reg.ecx + 1);
// (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)
// (Ways + 1) * (Partitions + 1) * (Line_Size + 1)* (Sets + 1)