#pragma once
#include "CBase.h"
#include "Engine_Define.h"

BEGIN(Engine)

class CGameObject;

class ENGINE_DLL CComponent : public CBase
{
protected:
	explicit CComponent();
	explicit CComponent(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CComponent(const CComponent& rhs);
	virtual ~CComponent();

public:
	virtual HRESULT Ready_Component() { return S_OK; }
	virtual void Update_Component(const _float& fTimeDelta) {}
	virtual void LateUpdate_Component() {}

protected:
	LPDIRECT3DDEVICE9			m_pGraphicDev;

protected:
	virtual void				Free()PURE;

public:
	CGameObject* m_pOwner = nullptr;
};

END

