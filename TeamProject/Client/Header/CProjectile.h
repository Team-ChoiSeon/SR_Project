#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CModel;
	class CTransform;
	class CRigidBody;
	class CCollider;
	class CParticle;
	class CPickTarget;
}

enum class EProjectileState{MSHOT, GROUND, HOLD, PSHOT};

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

	void Set_Grab(bool bGrab);
	void Set_CursorVec(const _vec3& cursorvec) { m_vCursorDelta = cursorvec; }
	void Pick(CTransform* pTransform);
	void Throw(const _vec3& vDir, const _float& fSpeed);
	EProjectileState Get_State() const { return m_eState; }

private:
	bool Check_Ray(_float& fDist);



private:
	CModel* m_pModel = nullptr;
	CTransform* m_pTransform = nullptr;
	CRigidBody* m_pRigid = nullptr;
	CCollider* m_pCol = nullptr;
	CParticle* m_pParticle = nullptr;

	CPickTarget* m_pPickTarget = nullptr;
	CTransform* m_pPickerTransform = nullptr;

	_float m_fLifeTime = 10.f;

	EProjectileState m_eState = EProjectileState::MSHOT;
	bool m_bGrab = false;
	_vec3 m_vCursorDelta;
};

