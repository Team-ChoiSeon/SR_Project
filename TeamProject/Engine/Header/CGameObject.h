#pragma once
#include "CBase.h"
#include "CComponent.h"
#include "CModel.h"
#include "CCamera.h"
#include "CUI.h"

BEGIN(Engine)



class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};
	/*virtual			void		Deserialize(const json& j);*/

public:
	// 사용 예시
	// CTransform* pTransform = Get_Component<CTransform>();
	template<typename T>
	T* Get_Component();

	// 사용 예시
	/*
	*	ID_DYNAMIC 이 default 
	*	Add_Component<CTransform>(m_pGraphicDev); 
	*	Add_Component<CRenderer>(ID_STATIC, m_pGraphicDev);
	*/
	template<typename T, typename... Args>
	void Add_Component(COMPONENTID eID = ID_DYNAMIC, Args&&... args);

	template<typename T>
	bool Has_Component();

protected:
	virtual		void		Free();

protected:
	unordered_map<type_index, CComponent*>			m_umComponent[ID_END];
	LPDIRECT3DDEVICE9								m_pGraphicDev;

};

template<typename T>
T* CGameObject::Get_Component()
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		auto iter = m_umComponent[i].find(typeid(T));
		if (iter != m_umComponent[i].end())
			return static_cast<T*>(iter->second);
	}
	MSG_BOX("[GameObject] Get_Component : ");
	return nullptr;
}

template<typename T, typename... Args>
void CGameObject::Add_Component(COMPONENTID eID, Args&&... args)
{
	const std::type_index tag = typeid(T);
	if (m_umComponent[eID].find(tag) != m_umComponent[eID].end())
	{
		MSG_BOX("[GameObject] Add_Component : ");
		return;
	}
  
  
	T* pComp = T::Create(std::forward<Args>(args)...);
	if (pComp == nullptr)
	{
		MSG_BOX("[GameObject] Add_Component : ");
		return;
	}
  
	pComp->m_pOwner = this;
	m_umComponent[eID].emplace(tag, pComp);
}


template<typename T>
bool CGameObject::Has_Component()
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		if (m_umComponent[i].find(typeid(T)) != m_umComponent[i].end())
			return true;
	}
	return false;
}
END