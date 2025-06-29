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
	// ��� ����
	// CRenderer* pRenderer = pGameObject->Get_Component<CRenderer>(L"Renderer");
	template<typename T = CComponent>
	T* Get_Component(const wstring& pComponentTag);

	// ��� ����
	/*
		CTransform* pTransform = new CTransform(...);
		pGameObject->Add_Component(L"Transform", pTransform);  // ID_DYNAMIC���� �ڵ� ���
		
		CRenderer* pRenderer = new CRenderer(...);
		pGameObject->Add_Component(L"Renderer", pRenderer, ID_STATIC);  // ��������� ID ������ ����
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

	MSG_BOX("[CGameObject] Get_Component ���� : nullptr ���޵�");
	return nullptr;
}

template<typename T, typename... Args>
void CGameObject::Add_Component(const wstring& pComponentTag, COMPONENTID eID, Args&&... args)
{
	unique_ptr<T> pComp (T::Create(forward<Args>(args)...));
	if (pComp == nullptr)
	{
		MSG_BOX("[GameObject] Add_Component ���� : ���� ����");
		return;
	}


	if (m_umComponent[eID].find(pComponentTag) != m_umComponent[eID].end())
	{
		MSG_BOX("[GameObject] Add_Component ����: �ߺ� �±�");
		return;
	}

	m_umComponent[eID].emplace(pComponentTag, pComp.release());
}



END