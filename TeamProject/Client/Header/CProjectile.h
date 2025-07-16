#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CModel;
	class CTransform;
	class CRigidBody;
	class CCollider;
}



class CProjectile : public CGameObject
{
private:
	explicit CProjectile(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CProjectile(const CProjectile& rhs);
	virtual ~CProjectile();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CProjectile* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;



private:
	CModel* m_pModel = nullptr;
	CTransform* m_pTransform = nullptr;
	CRigidBody* m_pRigid = nullptr;
	CCollider* m_pCol = nullptr;

	_float m_fLifeTime = 10.f;
};

