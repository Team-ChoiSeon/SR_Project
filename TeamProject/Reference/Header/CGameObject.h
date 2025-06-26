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

END