
#pragma once
#include <malloc.h>
#include <utility>
#include <type_traits>

namespace data_structure {
	template<size_t index>
	class intrusive_shared_pointer_hook {
	private:
		template<typename type, size_t>
		friend class intrusive_shared_pointer;
		struct reference final {
		private:
			using size_type = unsigned int;
		public:
			size_type _use;
			size_type _weak;
		};
	private:
		reference _reference;
	};

	template<typename type, size_t index>
	class intrusive_shared_pointer final {
	private:
		using size_type = unsigned int;
		using node = intrusive_shared_pointer_hook<index>;
		static_assert(std::is_base_of<node, type>::value);
	public:
		inline constexpr explicit intrusive_shared_pointer(void) noexcept
			: _node(nullptr) {
		}
		inline constexpr intrusive_shared_pointer(nullptr_t) noexcept
			: _node(nullptr) {
		};
		inline explicit intrusive_shared_pointer(type* value) noexcept {
			_node = static_cast<node*>(value);
			_node->_reference._use = 1;
			_node->_reference._weak = 0;
		}
		inline intrusive_shared_pointer(intrusive_shared_pointer const& rhs) noexcept
			: _node(rhs._node) {
			if (nullptr != _node)
				++_node->_reference._use;
		};
		inline explicit intrusive_shared_pointer(intrusive_shared_pointer&& rhs) noexcept
			: _node(rhs._node) {
			rhs._node = nullptr;
		};
		inline auto operator=(intrusive_shared_pointer const& rhs) noexcept -> intrusive_shared_pointer& {
			intrusive_shared_pointer(rhs).swap(*this);
			return *this;
		}
		inline auto operator=(intrusive_shared_pointer&& rhs) noexcept -> intrusive_shared_pointer& {
			intrusive_shared_pointer(std::move(rhs)).swap(*this);
			return *this;
		};
		inline ~intrusive_shared_pointer(void) noexcept {
			if (nullptr != _node && 0 == --_node->_reference._use) {
				destructor(static_cast<type*>(_node));
				//if constexpr (!std::is_trivially_destructible_v<type>) 
				//	_value->~type(); // 소멸자 호출
				//free(_value); // 벨류는 지움
				//if (0 == _reference->_weak) // 위크 포인터가 0이면 레퍼런스까지 같이 지움
				//	free(_reference); //
			}
		}
	public:
		inline auto operator*(void) noexcept -> type& {
			return static_cast<type&>(*_node);
		}
		inline auto operator->(void) noexcept -> type* {
			return static_cast<type*>(_node);
		}
	public:
		inline void swap(intrusive_shared_pointer& rhs) noexcept {
			auto temp = _node;
			_node = rhs._node;
			rhs._node = temp;
		}
		inline auto use_count(void) const noexcept -> size_type {
			return _node->_reference._use;
		}
		inline auto get(void) const noexcept -> type* {
			return static_cast<type*>(_node);
		}
		inline auto data(void) noexcept -> node* {
			return _node;
		}
		inline auto reset(void) noexcept {
			_node = nullptr;
		}
	private:
		node* _node;
	};

	template <class type, size_t index>
	inline bool operator==(const intrusive_shared_pointer<type, index>& value, nullptr_t) noexcept {
		return value.get() == nullptr;
	}

}
