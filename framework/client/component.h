#pragma once
#include "library/container/list.h"

namespace framework {
	class object;
	class system_interface;
	class component {
		using size_type = unsigned int;
		inline static size_type _static_type_id = 0;
	protected:
		size_type _type_id;
		object* _object;
		library::list<system_interface*> _system;
	public:
		inline explicit component(size_type type_id) noexcept
			: _type_id(type_id) {
		};
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
		inline auto get_type_id(void) const noexcept -> size_type {
			return _type_id;
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