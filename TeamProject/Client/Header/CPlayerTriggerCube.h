#pragma once
#include "CGameObject.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "DummyCube.h"

namespace Engine {
	class CTransform;
	class CModel;
	class CCollider;
}
class CPlayerTriggerCube : public CGameObject
{
private:
	explicit CPlayerTriggerCube(LPDIRECT3DDEVICE9 pGrpahicDev);
	virtual ~CPlayerTriggerCube();

public:

	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CPlayerTriggerCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

	bool Get_InGate() { return m_bInGate; };

private:
	CTransform* m_pTransform;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	bool m_bInGate = false;
};

