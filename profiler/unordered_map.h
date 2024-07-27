#pragma once
#include <iostream>
#include "pair.h"
#include "hash.h"

#include "list.h"
#include "vector.h"

template<typename _Kty, typename _Ty, class hash = hash<_Kty>>
class unordered_map final {
	using pair = pair<_Kty, _Ty>;
	using iterator = typename list<pair>::iterator;
public:
	inline explicit unordered_map(void) noexcept {
		rehash(_count);
	}
	inline ~unordered_map(void) noexcept = default;
public:
	template<typename _Uy>
	inline void insert(_Kty const& key, _Uy&& value) const noexcept {
		emplace(key, std::forward<_Uy>(value));
	}
	template<typename... _Arg>
	inline auto emplace(_Kty const& key, _Arg&&... argument) noexcept -> iterator {
		auto iter = find(key);
		if (iter != _list.end())
			return iter;

		size_t idx = bucket(key);
		auto& first = _vec[idx << 1];
		auto& last = _vec[(idx << 1) + 1];

		auto res = _list.emplace(first, key, std::forward<_Arg>(argument)...);
		if (first == _list.end())
			last = res;
		first = res;

		if (1.f <= load_factor())
			rehash(_count < 512 ? _count * 8 : _count + 1);
		return res;
	}
	inline void erase(_Kty const& key) noexcept {
		auto iter = find(key);
		if (iter == _list.end())
			return;
		erase(iter);
	}
	inline void erase(iterator const& iter) noexcept {
		size_t idx = bucket((*iter)._first);

		auto& first = _vec[idx << 1];
		auto& last = _vec[(idx << 1) + 1];

		if (first == last)
			first = last = _list.end();
		else if (first == iter)
			++first;
		else if (last == iter)
			--last;
		_list.erase(iter);
	}
	inline auto operator[](_Kty const& key) noexcept -> _Ty& {
		return (*emplace(key))._second;
	}
public:
	inline auto begin(void) const noexcept -> typename iterator {
		return _list.begin();
	}
	inline auto begin(size_t const idx) const noexcept -> iterator {
		return _vec[idx << 1];
	}
	inline auto end(void) const noexcept -> typename iterator {
		return _list.end();
	}
	inline auto end(size_t const idx) const noexcept -> iterator {
		auto iter = _vec[(idx << 1) + 1];
		if (_list.end() != iter)
			iter++;
		return iter;
	}
public:
	inline auto size(void) const noexcept -> size_t {
		return _list.size();
	}
	inline auto size(size_t const idx) const noexcept -> size_t {
		//bucket_size
	}
	inline bool empty(void) const noexcept {
		return  _list.empty();
	}
	inline auto count(void) const noexcept -> size_t {
		return _count;
	}
	inline auto bucket(_Kty const& key) const noexcept -> size_t {
		return _hash(key) % _count;
	}
	inline void clear(void) noexcept {
		_vec.assign(_count << 1, _list.end());
		_list.clear();
	}
	inline auto find(_Kty const& key) const noexcept -> iterator {
		size_t idx = bucket(key);

		auto first = begin(idx);
		auto last = end(idx);
		auto end = _list.end();

		if (first == end)
			return end;

		for (auto iter = first;; ++iter) {
			if ((*iter)._first == key)
				return iter;
			if (iter == last)
				break;
		}
		return end;
	}
public:
	inline auto load_factor(void) const noexcept -> float {
		return static_cast<float>(_list.size()) / _count;
	}
	inline void rehash(size_t const count) noexcept {
		unsigned long bit;
		_BitScanReverse64(&bit, ((count - 1) | 1));
		_count = static_cast<size_t>(1) << (1 + bit);

		auto iter = _list.begin();
		auto end = _list.end();
		_vec.assign(_count << 1, end);

		for (auto next = iter; iter != end; iter = next) {
			++next;
			auto idx = bucket((*iter)._first);

			auto& first = _vec[idx << 1];
			auto& last = _vec[(idx << 1) + 1];

			if (first == end)
				last = iter;
			else
				_list.splice(first, iter, next);
			first = iter;
		}
	}
private:
	vector<iterator> _vec;
	list<pair> _list;
	size_t _count = 8;
	hash _hash;
};