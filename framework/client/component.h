#pragma once
#include "library/container/list.h"
#include "library/container/pool.h"
#include "library/pattern/singleton.h"
#include "library/container/intrusive/share_pointer.h"

namespace framework {
	class object;
	class system_interface;
	
	class component {
		using size_type = unsigned int;
		inline static size_type _static_type_id = 0;
	protected:
		size_type _type_id;
		library::intrusive::weak_pointer<object, 0> _object;
		library::list<system_interface*> _system;
	public:
		explicit component(size_type const type_id, library::intrusive::share_pointer<object, 0>& object) noexcept;
		inline explicit component(component const&) noexcept = delete;
		inline explicit component(component&&) noexcept = delete;
		inline auto operator=(component const&) noexcept -> component & = delete;
		inline auto operator=(component&&) noexcept -> component & = delete;
		inline virtual ~component(void) noexcept = default;

		template<typename type>
		inline static auto type_id(void) noexcept -> size_type {
			static size_type type_id = _static_type_id++;
			return type_id;
		}
		inline auto type_id(void) const noexcept -> size_type;
	};
}

namespace implement {
	class component_pool_interface {
	protected:
		using size_type = unsigned int;
	public:
		inline virtual ~component_pool_interface(void) = default;

		inline virtual auto allocate(void) noexcept -> framework::component* = 0;
		inline virtual auto allocate(framework::component* value) noexcept -> framework::component* = 0;
		inline virtual void deallocate(framework::component* const value) noexcept = 0;
	};
	template <typename type>
	class component_pool : public component_pool_interface, public library::pool<type> {
		using base = library::pool<type>;
		using size_type = component_pool_interface::size_type;
	public:
		using base::base;
		inline virtual ~component_pool(void) = default;

		inline virtual auto allocate(void) noexcept -> framework::component* override {
			return base::allocate();
		};
		inline virtual auto allocate(framework::component* value) noexcept -> framework::component* override {
			return base::allocate(*value);
		}
		inline virtual void deallocate(framework::component* const value) noexcept override {
			base::deallocate(value);
		}
	};
	class component_manager : public library::singleton<component_manager> {
		friend class library::singleton<component_manager>;
		using size_type = unsigned int;
		library::unorder_map<size_type, component_pool_interface*> _component;

		inline explicit component_manager(void) noexcept = default;
		inline explicit component_manager(component_manager const&) noexcept = delete;
		inline explicit component_manager(component_manager&&) noexcept = delete;
		inline auto operator=(component_manager const&) noexcept -> component_manager & = delete;
		inline auto operator=(component_manager&&) noexcept -> component_manager & = delete;
		inline ~component_manager(void) noexcept = default;
	public:
		template<typename type>
		inline auto create_component(void) noexcept -> type& {
			auto result = _component.find(framework::component::type_id<type>());
			if (_component.end() == result)
				result = _component.emplace(framework::component::type_id<type>(), new component_pool<type>);
			return result->_second->allocate();
		}
		inline auto clone_component(framework::component* component) noexcept {
			auto result = _component.find(component->type_id());
			if (_component.end() == result)
				__debugbreak();
			result->_second->allocate(component);
		}
	};
}


//#pragma once
//#include "Base.h"
//
//BEGIN_NAMESPACE(Engine)
//class DECLSPEC_DLL CComponent abstract : public CBase {
//public:
//	virtual CComponent* const Clone(void* const& pArg = nullptr) PURE;
//protected:
//	explicit CComponent(ID3D11Device* const& pDevice, ID3D11DeviceContext* const& pContext);
//	explicit CComponent(const CComponent& rhs);
//	virtual ~CComponent(void) override = default;
//	virtual void Delete(void) override;
//public:
//	virtual void Update_Edit(const _int& iPushID) {};
//public:
//	virtual void Save_Data(Json::Value& root) {};
//	virtual void Load_Data(Json::Value& root) {};
//protected:
//	ID3D11Device* m_pDevice = nullptr;
//	ID3D11DeviceContext* m_pContext = nullptr;
//	_bool m_bClone = false;
//};
//END_NAMESPACE