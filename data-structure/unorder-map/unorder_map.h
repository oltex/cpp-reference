#pragma once
#include "../pair/pair.h"
#include "../list/list.h"
#include "../vector/vector.h"
#include "../../function/function.h"

struct piecewise_construct_t {
	explicit piecewise_construct_t() = default;
};
constexpr piecewise_construct_t piecewise_construct{};

namespace library {
	template<typename key_type, typename type, auto hash = fnv_hash<key_type>>
	class unorder_map final {
	private:
		using size_type = unsigned int;
		using pair = pair<key_type, type>;
	public:
		using iterator = typename list<pair>::iterator;
	private:
		template <typename key_type, typename... argument>
		struct extract {
			static constexpr bool able = false;
		};
		template <typename key_type, typename type>
		struct extract<key_type, key_type, type> {
			static constexpr bool able = true;
			static auto execute(key_type const& key, type const&) noexcept -> key_type const& {
				return key;
			}
		};
		vector<iterator> _vector;
		list<pair> _list;
	public:
		inline explicit unorder_map(void) noexcept {
			rehash(8);
		}
		inline explicit unorder_map(unorder_map const& rhs) noexcept;
		inline explicit unorder_map(unorder_map&& rhs) noexcept;
		inline auto operator=(unorder_map const& rhs) noexcept -> unorder_map&;
		inline auto operator=(unorder_map&& rhs) noexcept -> unorder_map&;
		inline ~unorder_map(void) noexcept = default;
	public:
		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept /*-> iterator*/ {
			iterator current;

			using extract = extract<key_type, std::remove_cvref_t<argument>...>;
			if constexpr (true == extract::able) {
				auto const& value = extract::execute(std::forward<argument>(arg)...);

				auto index = bucket(value);
				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first != _list.end()) {
					for (auto iter = first;; ++iter) {
						if ((*iter)._first == value)
							return iter;
						if (iter == last)
							break;
					}
				}

				current = _list.emplace(first, std::forward<argument>(arg)...);
				if (first == _list.end())
					last = current;
				first = current;
			}
			else {
				assert((std::is_constructible_v<pair, argument...>));
				current = iterator(_list.allocate(std::forward<argument>(arg)...));

				auto index = bucket((*current)._first);
				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first != _list.end()) {
					for (auto iter = first;; ++iter) {
						if ((*iter)._first == (*current)._first) {
							_list.deallocate(current._node);
							return iter;
						}
						if (iter == last)
							break;
					}
				}

				_list.link(first._node, current._node);
				if (first == _list.end())
					last = current;
				first = current;
			}

			if (1.f <= load_factor())
				rehash((_vector.size() >> 1) < 512 ? (_vector.size() >> 1) * 8 : (_vector.size() >> 1) + 1);
			return current;
		}
		inline void erase(key_type const& key) noexcept {
			assert(_list.size() > 0 && "called on empty");
			erase(find(key));
		}
		inline void erase(iterator const& iter) noexcept {
			size_type index = bucket((*iter)._first);

			auto& first = _vector[index << 1];
			auto& last = _vector[(index << 1) + 1];

			if (first == last)
				first = last = _list.end();
			else if (first == iter)
				++first;
			else if (last == iter)
				--last;
			_list.erase(iter);
		}
		inline auto operator[](key_type const& key) noexcept -> type& {
			return (*emplace(key))._second;
		}
		inline auto begin(void) const noexcept -> typename iterator {
			return _list.begin();
		}
		inline auto begin(size_type const index) const noexcept -> iterator {
			return _vector[index << 1];
		}
		inline auto end(void) const noexcept -> typename iterator {
			return _list.end();
		}
		inline auto end(size_type const index) const noexcept -> iterator {
			auto iter = _vector[(index << 1) + 1];
			if (_list.end() != iter)
				++iter;
			return iter;
		}

		inline auto load_factor(void) const noexcept -> float {
			return static_cast<float>(_list.size()) / (_vector.size() >> 1);
		}
		inline auto bucket(key_type const& key) const noexcept -> size_type {
			return hash(key) % (_vector.size() >> 1);
		}
		inline auto bucket_count(void) const noexcept -> size_type {
			return (_vector.size() >> 1);
		}
		//inline auto bucket_size(size_type const index) const noexcept -> size_type;

		inline void rehash(size_type count) noexcept {
			unsigned long bit;
			_BitScanReverse64(&bit, ((count - 1) | 1));
			count = static_cast<size_type>(1) << (1 + bit);

			auto begin = _list.begin();
			auto end = _list.end();
			_vector.assign(count << 1, end);

			while (begin != end) {
				auto current = begin++;

				auto index = bucket((*current)._first);

				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first == end)
					last = current;
				else
					_list.splice(first._node, current._node, begin._node);
				first = current;
			}
		}
		inline auto find(key_type const& key) const noexcept -> iterator {
			size_type index = bucket(key);

			auto end = _list.end();
			auto first = _vector[index << 1];
			auto last = _vector[(index << 1) + 1];

			if (first != end) {
				for (auto iter = first;; ++iter) {
					if ((*iter)._first == key)
						return iter;
					if (iter == last)
						break;
				}
			}
			return end;
		}
		inline void clear(void) noexcept {
			_vector.assign(_vector.size(), _list.end());
			_list.clear();
		}
		inline auto size(void) const noexcept -> size_type {
			return _list.size();
		}
		inline bool empty(void) const noexcept {
			return  _list.empty();
		}
	};
}