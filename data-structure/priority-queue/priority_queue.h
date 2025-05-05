#pragma once
#include "../vector/vector.h"
#include "../../algorithm/predicate/predicate.h"

namespace library::data_structure {
	template<typename type, auto _predicate = algorithm::predicate::less<type>>
	class priority_queue final {
	private:
		using size_type = unsigned int;
	public:
		inline explicit priority_queue(void) noexcept = default;
		inline explicit priority_queue(priority_queue const& rhs) noexcept;
		inline explicit priority_queue(priority_queue&& rhs) noexcept;
		inline auto operator=(priority_queue const& rhs) noexcept -> priority_queue&;
		inline auto operator=(priority_queue&& rhs) noexcept -> priority_queue&;
		inline ~priority_queue(void) noexcept = default;

		template<typename... argument>
		inline void emplace(argument&&... arg) noexcept {
			_vector.emplace_back(std::forward<argument>(arg)...);
			auto leaf = _vector.back();
			auto child = _vector.size() - 1;
			while (0 < child) {
				auto parent = (child - 1) / 2;

				if (0 > _predicate(_vector[parent], leaf))
					break;
				_vector[child] = _vector[parent];
				child = parent;
			}
			_vector[child] = leaf;
		};
		inline void pop(void) noexcept {
			auto leaf = _vector.back();
			auto size = _vector.size() - 1;

			size_type parent = 0;
			for (;;) {
				auto left = parent * 2 + 1;
				if (size <= left)
					break;
				auto right = left + 1;

				if (size > right && 0 > _predicate(_vector[right], _vector[left]))
					left = right;
				if (0 > _predicate(leaf, _vector[left]))
					break;

				_vector[parent] = _vector[left];
				parent = left;
			}
			_vector[parent] = leaf;
			_vector.pop_back();
		}

		inline auto top(void) const noexcept -> type& {
			return _vector.front();
		};
		inline auto size(void) const noexcept -> size_type {
			return _vector.size();
		}
		inline bool empty(void) const noexcept {
			return _vector.empty();
		}
		inline void clear(void) noexcept {
			_vector.clear();
		}
	private:
		vector<type> _vector;
	};
}