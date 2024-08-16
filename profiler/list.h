#pragma once
#include <utility>
#include <stdlib.h>
#include <malloc.h>

template<typename _Ty>
class list final {
private:
	struct node final {
		_Ty _value;
		node* _prev, * _next;
	};
public:
	class iterator final {
	public:
		inline explicit iterator(node* const node = nullptr) noexcept
			: _node(node) {
		}
		inline iterator(iterator const& rhs) noexcept
			: _node(rhs._node) {
		}
		inline iterator& operator=(iterator const& rhs) noexcept {
			_node = rhs._node;
			return *this;
		}
	public:
		inline auto operator*(void) const noexcept -> _Ty& {
			return _node->_value;
		}
		inline auto operator++(void) noexcept -> iterator& {
			_node = _node->_next;
			return *this;
		}
		inline auto operator++(int) noexcept -> iterator {
			iterator iter(*this);
			_node = _node->_next;
			return iter;
		}
		inline auto operator--(void) noexcept -> iterator& {
			_node = _node->_prev;
			return *this;
		}
		inline auto operator--(int) noexcept -> iterator {
			iterator iter(*this);
			_node = _node->_prev;
			return iter;
		}
		inline bool operator==(iterator const& rhs) const noexcept {
			return _node == rhs._node;
		}
		inline bool operator!=(iterator const& rhs) const noexcept {
			return _node != rhs._node;
		}
	public:
		node* _node;
	};
public:
	inline explicit list(void) noexcept {
		_head = static_cast<node*>(calloc(1, sizeof(node)));
#pragma warning(suppress: 6011)
		_head->_next = _head->_prev = _head;
	}
	inline list(std::initializer_list<_Ty> init_list) noexcept
		: list() {
		for (auto& iter : init_list)
			emplace_back(iter);
	}
	inline explicit list(iterator const& begin, iterator const& end) noexcept
		: list() {
		for (auto iter = begin; iter != end; ++iter)
			emplace_back(*iter);
	}
	inline explicit list(list<_Ty> const& rhs) noexcept
		: list(rhs.begin(), rhs.end()) {
	}
	inline explicit list(list<_Ty>&& rhs) noexcept
		: list() {
		swap(rhs);
	}
	inline ~list(void) noexcept {
		clear();
		free(_head);
	}
public:
	template<typename _Uy>
	inline void push_front(_Uy&& value) noexcept {
		emplace(begin(), std::forward<_Uy>(value));
	}
	template<typename _Uy>
	inline void push_back(_Uy&& value) noexcept {
		emplace(end(), std::forward<_Uy>(value));
	}
	template<typename _Uy>
	inline auto insert(iterator const& iter, _Uy&& value) noexcept -> iterator {
		return emplace(iter, std::forward<_Uy>(value));
	}
	template<typename... _Arg>
	inline auto emplace_front(_Arg&&... argument) noexcept -> _Ty& {
		return *emplace(begin(), std::forward<_Arg>(argument)...);
	}
	template<typename... _Arg>
	inline auto emplace_back(_Arg&&... argument) noexcept -> _Ty& {
		return *emplace(end(), std::forward<_Arg>(argument)...);
	}
	template<typename... _Arg>
	inline auto emplace(iterator const& iter, _Arg&&... argument) noexcept -> iterator {
		auto cur = static_cast<node*>(malloc(sizeof(node)));
#pragma warning(suppress: 6011)
		new(&cur->_value) _Ty(std::forward<_Arg>(argument)...);

		auto next = iter._node;
		auto prev = next->_prev;

		prev->_next = cur;
		cur->_prev = prev;
		cur->_next = next;
		next->_prev = cur;

		++_size;
		return iterator(cur);
	}
	inline void pop_front(void) noexcept {
		erase(begin());
	}
	inline void pop_back(void) noexcept {
		erase(--end());
	}
	inline auto erase(iterator const& iter) noexcept -> iterator {
		auto cur = iter._node;
		auto prev = cur->_prev;
		auto next = cur->_next;

		prev->_next = next;
		next->_prev = prev;

		cur->_value.~_Ty();
		free(cur);
		--_size;
		return iterator{ next };
	}
public:
	inline auto begin(void) const noexcept -> iterator {
		return iterator(_head->_next);
	}
	inline auto end(void) const noexcept -> iterator {
		return iterator(_head);
	}
public:
	inline auto size(void) const noexcept -> size_t {
		return _size;
	}
	inline bool empty(void) const noexcept {
		return 0 == _size;
	}
	inline void clear(void) noexcept {
		auto cur = _head->_next;
		for (auto next = cur; cur != _head; cur = next) {
			next = next->_next;
			cur->_value.~_Ty();
			free(cur);
		}
		_head->_next = _head->_prev = _head;
		_size = 0;
	}
	inline void swap(list& rhs) noexcept {
		node* node = _head;
		_head = rhs._head;
		rhs._head = node;

		size_t size = _size;
		_size = rhs._size;
		rhs._size = size;
	}
	inline void splice(iterator const& _before, iterator const& _first, iterator const& _last) noexcept {
		node* before = _before._node;
		node* first = _first._node;
		node* last = _last._node;

		node* first_prev = first->_prev;
		first_prev->_next = last;
		node* last_prev = last->_prev;
		last_prev->_next = before;
		node* before_prev = before->_prev;
		before_prev->_next = first;

		before->_prev = last_prev;
		last->_prev = first_prev;
		first->_prev = before_prev;

		//return last;
	}
private:
	node* _head;
	size_t _size = 0;
};