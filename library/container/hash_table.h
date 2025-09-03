#pragma once
#include "../function.h"
#include "../template.h"
#include "pair.h"
#include "tuple.h"
#include "list.h"
#include "vector.h"

namespace detail {
	template<typename type>
	class unorder_set {
	protected:
		using key_type = type;
		using value_type = type;
		using element = type;
		using iterator = typename library::list<element>::iterator;

		template <typename... argument>
		using key_exist = library::key_exist_set<type, argument...>;
		inline static auto key_extract(element const& elem) noexcept -> type const& {
			return elem;
		}
		inline static auto value_extract(element const& elem) noexcept -> type const& {
			return elem;
		}
	};
	template<typename _key_type, typename _value_type>
	class unorder_map {
	protected:
		using key_type = _key_type;
		using value_type = _value_type;
		using element = library::pair<key_type, value_type>;
		using iterator = typename library::list<element>::iterator;

		template <typename... argument>
		using key_exist = library::key_exist_map<key_type, argument...>;
		inline static auto key_extract(element const& elem) noexcept -> key_type const& {
			return elem._first;
		}
		inline static auto value_extract(element const& elem) noexcept -> value_type const& {
			return elem._second;
		}
	};

	template <typename trait, auto hash = library::fnv_hash<trait::key_type>>
	class hash_table final : public trait {
		using size_type = unsigned int;
		using key_type = trait::key_type;
		using value_type = trait::value_type;
		using element = trait::element;
		using iterator = trait::iterator;

		library::vector<iterator> _vector;
		library::list<element> _list;
	public:
		inline explicit hash_table(void) noexcept {
			rehash(8);
		}
		inline explicit hash_table(hash_table const& rhs) noexcept;
		inline explicit hash_table(hash_table&& rhs) noexcept;
		inline auto operator=(hash_table const& rhs) noexcept -> hash_table&;
		inline auto operator=(hash_table&& rhs) noexcept -> hash_table&;
		inline ~hash_table(void) noexcept = default;

		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept -> iterator {
			iterator current;

			using exist = typename trait::template key_exist<std::remove_cvref_t<argument>...>;
			if constexpr (true == exist::able) {
				auto const& value = exist::execute(std::forward<argument>(arg)...);

				auto index = bucket(value);
				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first != _list.end()) {
					for (auto iter = first;; ++iter) {
						if (trait::key_extract(*iter) == value)
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
				assert((std::is_constructible_v<element, argument...>));
				current = iterator(_list.allocate(std::forward<argument>(arg)...));

				auto index = bucket((*current)._first);
				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first != _list.end()) {
					for (auto iter = first;; ++iter) {
						if (trait::key_extract(*iter) == trait::key_extract(*current)) {
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
		inline auto operator[](key_type const& key) noexcept -> value_type& {
			return trait::value_extract(*emplace(key));
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
			unsigned long bit =  library::bit_scan_reverse((count - 1) | 1);
			count = static_cast<size_type>(1) << (1 + bit);

			auto begin = _list.begin();
			auto end = _list.end();
			_vector.assign(count << 1, end);

			while (begin != end) {
				auto current = begin++;

				auto index = bucket(trait::key_extract(*current));

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
					if (trait::key_extract(*iter) == key)
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
			return _list.empty();
		}
	};
}

namespace library {
	template <typename type, auto hash = library::fnv_hash<type>>
	using unorder_set = detail::hash_table<detail::unorder_set<type>, hash>;
	template <typename key_type, typename value_type, auto hash = library::fnv_hash<key_type>>
	using unorder_map = detail::hash_table<detail::unorder_map<key_type, value_type>, hash>;
}