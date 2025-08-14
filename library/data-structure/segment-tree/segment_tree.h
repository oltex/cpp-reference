#pragma once
#include "../vector/vector.h"
#include "../../function/function.h"
#include <initializer_list>

namespace library {
	template<typename type>
	class segment_tree final {
		using size_type = unsigned int;
		vector<type> _vector;
		size_type _size;
	public:
		inline explicit segment_tree(std::initializer_list<type> list) noexcept {
			_size = bit_ceil(static_cast<size_type>(list.size()));
			_vector.resize(_size * 2, 0);

			auto vec = &_vector[_size];
			for (auto& iter : list)
				*vec++ = iter;

			for (auto index = _size - 1; index > 0; --index) {
				auto left = 2 * index;
				_vector[index] = _vector[left] + _vector[left + 1];
			}
		}
		inline explicit segment_tree(segment_tree const& rhs) noexcept;
		inline explicit segment_tree(segment_tree&& rhs) noexcept;
		inline auto operator=(segment_tree const& rhs) noexcept -> segment_tree&;
		inline auto operator=(segment_tree&& rhs) noexcept -> segment_tree&;
		inline ~segment_tree(void) noexcept = default;

		inline void update(size_type index, type const value) noexcept {
			index += _size;
			_vector[index] = value;

			while (1 < index) {
				index = index / 2;
				auto left = index * 2;
				_vector[index] = _vector[left] + _vector[left + 1];
			}
		}
		inline auto query(size_type left, size_type right) noexcept -> type {
			left += _size;
			right += _size;
			type result = 0;
			while (left <= right) {
				if (left % 2 == 1)
					result += _vector[left++];
				if (right % 2 == 0)
					result += _vector[right--];
				left /= 2;
				right /= 2;
			}
			return result;
		}

		inline auto query_recursion(size_type left, size_type right) const noexcept -> type {
			return query_recursion(1, 0, _size - 1, left, right);
		}
		inline auto query_recursion(size_type node, size_type node_left, size_type node_right, size_type query_left, size_type query_right) const noexcept {
			if (query_right < node_left || node_right < query_left)
				return 0;
			if (query_left <= node_left && node_right <= query_right)
				return _vector[node];

			size_type mid = (node_left + node_right) / 2;
			return query_recursion(node * 2, node_left, mid, query_left, query_right) +
				query_recursion(node * 2 + 1, mid + 1, node_right, query_left, query_right);
		}
	};
}