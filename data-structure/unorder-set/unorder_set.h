#pragma once
#include "../list/list.h"
#include "../vector/vector.h"
#include "../../function/function.h"

namespace library {
	template<typename type, auto hash = fnv_hash<type>>
	class unorder_set final {
	public:
		using iterator = typename list<type>::iterator;
	private:
		using size_type = unsigned int;
		template <typename type, typename... argument>
		struct extract {
			static constexpr bool able = false;
		};
		template <typename type>
		struct extract<type, type> {
			static constexpr bool able = true;
			static auto execute(type const& value) noexcept -> type const& {
				return value;
			}
		};
		vector<iterator> _vector;
		list<type> _list;
	public:
		inline explicit unorder_set(void) noexcept {
			rehash(8);
		}
		inline explicit unorder_set(unorder_set const& rhs) noexcept;
		inline explicit unorder_set(unorder_set&& rhs) noexcept;
		inline auto operator=(unorder_set const& rhs) noexcept -> unorder_set&;
		inline auto operator=(unorder_set&& rhs) noexcept -> unorder_set&;
		inline ~unorder_set(void) noexcept = default;

		template<typename... argument>
		inline auto emplace(argument&&... arg) noexcept -> iterator {
			iterator current;
			using extract = extract<type, std::remove_cvref_t<argument>...>;
			if constexpr (true == extract::able) {
				auto const& value = extract::execute(std::forward<argument>(arg)...);

				auto index = bucket(value);
				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first != _list.end()) {
					for (auto iter = first;; ++iter) {
						if (*iter == value)
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
				current = iterator(_list.allocate(std::forward<argument>(arg)...));

				auto index = bucket(*current);
				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first != _list.end()) {
					for (auto iter = first;; ++iter) {
						if (*iter == *current) {
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
		inline void erase(type const& value) noexcept {
			assert(_list.size() > 0 && "called on empty");
			erase(find(value));
		}
		inline void erase(iterator const& iter) noexcept {
			size_type index = bucket(*iter);

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
		inline auto operator[](type const& value) noexcept -> type& {
			return (*emplace(value))._second;
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
		inline auto bucket(type const& value) const noexcept -> size_type {
			return hash(value) % (_vector.size() >> 1);
		}
		inline auto bucket_count(void) const noexcept -> size_type {
			return (_vector.size() >> 1);
		}
		//inline auto bucket_size(size_type index) const noexcept -> size_type {
		//}
		inline void rehash(size_type count) noexcept {
			unsigned long bit;
			_BitScanReverse64(&bit, ((count - 1) | 1));
			count = static_cast<size_type>(1) << (1 + bit);

			auto begin = _list.begin();
			auto end = _list.end();
			_vector.assign(count << 1, end);

			while (begin != end) {
				auto current = begin++;

				auto index = bucket(*current);

				auto& first = _vector[index << 1];
				auto& last = _vector[(index << 1) + 1];

				if (first == end)
					last = current;
				else
					_list.splice(first._node, current._node, begin._node);
				first = current;
			}
		}
		inline auto find(type const& value) const noexcept -> iterator {
			auto index = bucket(value);

			auto end = _list.end();
			auto first = _vector[index << 1];
			auto last = _vector[(index << 1) + 1];

			if (first != end) {
				for (auto iter = first;; ++iter) {
					if (*iter == value)
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