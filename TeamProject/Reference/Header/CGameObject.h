#pragma once
#include "CBase.h"
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CGameObject : public CBase
{
protected:
	explicit CGameObject(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CGameObject(const CGameObject& rhs);
	virtual ~CGameObject();

public:
	// 사용 예시
	// CRenderer* pRenderer = pGameObject->Get_Component<CRenderer>(L"Renderer");
	template<typename T = CComponent>
	T* Get_Component(const wstring& pComponentTag);

	// 사용 예시
	/*
		CTransform* pTransform = new CTransform(...);
		pGameObject->Add_Component(L"Transform", pTransform);  // ID_DYNAMIC으로 자동 등록
		
		CRenderer* pRenderer = new CRenderer(...);
		pGameObject->Add_Component(L"Renderer", pRenderer, ID_STATIC);  // 명시적으로 ID 지정도 가능
	*/
	template<typename T, typename... Args>
	void Add_Component(const wstring& pComponentTag, COMPONENTID eID, Args&&... args);

public:
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() = 0;

protected:
	unordered_map<wstring, CComponent*>			m_umComponent[ID_END];
	LPDIRECT3DDEVICE9						m_pGraphicDev;

protected:
	virtual		void		Free();
};
template<typename T>
T* CGameObject::Get_Component(const wstring& pComponentTag)
{
	for (_uint i = 0; i < ID_END; ++i)
	{
		auto iter = m_umComponent[i].find(pComponentTag);
		if (iter != m_umComponent[i].end())
			return dynamic_cast<T*>(iter->second);
	}

	MSG_BOX("[CGameObject] Get_Component 실패 : nullptr 전달됨");
	return nullptr;
}

template<typename T, typename... Args>
void CGameObject::Add_Component(const wstring& pComponentTag, COMPONENTID eID, Args&&... args)
{
	unique_ptr<T> pComp (T::Create(forward<Args>(args)...));
	if (pComp == nullptr)
	{
		MSG_BOX("[GameObject] Add_Component 실패 : 생성 실패");
		return;
	}


	if (m_umComponent[eID].find(pComponentTag) != m_umComponent[eID].end())
	{
		MSG_BOX("[GameObject] Add_Component 실패: 중복 태그");
		return;
	}

	m_umComponent[eID].emplace(pComponentTag, pComp.release());
}



END