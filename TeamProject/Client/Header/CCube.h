#pragma once
#include "CGameObject.h"

namespace Engine {
	class CModel;
	class CTransform;
}
class CCube : public CGameObject
{
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCube();

public:
	//Basic Function
	virtual HRESULT Ready_GameObject() override = 0;
	virtual _int Update_GameObject(const _float& fTimeDelta) override = 0;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override = 0;

	//Create, Release Function
	virtual void Free() = 0;

protected:
	CModel* m_pModel;
	CTransform* m_pTransform;
};

