#pragma once
#include "singleton.h"
#include <memory>
#include <intrin.h>

class cache_simulator final : public singleton<cache_simulator>{
	friend class singleton<cache_simulator>;
private:
	struct cache_line {
		unsigned int _tag;
		int _access = 0;
	};
private:
	inline explicit cache_simulator(void) noexcept {
		int reg[4];
		__cpuidex(reg, 0x00000004, 0x00000000);

		_set = reg[2] + 1;
		_way = ((reg[1] & 0xffc00000) >> 22) + 1;
		size_t line = (reg[1] & 0xfff) + 1;

		_hash = (cache_line**)malloc(sizeof(cache_line*) * _set);
		for (auto i = 0; i < _set; ++i)
			_hash[i] = (cache_line*)malloc(sizeof(cache_line*) * _way);

		_BitScanReverse(&_line, line);
		_mask = _set - 1 << _line;

		_BitScanReverse(&_tag, _set);
		_tag += _line;
	}
	~cache_simulator(void) noexcept {
		for (auto i = 0; i < _set; i++)
			free(_hash[i]);
		free(_hash);
	}
public:
	void push(void* ptr) {
		auto idx = ((size_t)ptr & _mask) >> _line;
		auto tag = (size_t)ptr >> _tag;

		//for (auto i = 0; i < _way; ++i) {
			//_hash[idx][i]._tag == tag
		//}
	}
public:
	cache_line** _hash;
	size_t _set;
	size_t _way;

	size_t _mask;
	unsigned long _line;
	unsigned long _tag;
};

// (((reg.ebx & 0xffc00000) >> 22) + 1) * (((reg.ebx & 0x3ff000) >> 12) + 1) * ((reg.ebx & 0xfff) + 1) * (reg.ecx + 1);
// (EBX[31:22] + 1) * (EBX[21:12] + 1) * (EBX[11:0] + 1) * (ECX + 1)
// (Ways + 1) * (Partitions + 1) * (Line_Size + 1)* (Sets + 1)