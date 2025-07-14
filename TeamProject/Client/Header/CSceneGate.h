#pragma once
#include "CGameObject.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "DummyCube.h"
#include "CTestTile.h"
#include "CPlayerTriggerCube.h"


namespace Engine {
	class CTransform;
	class CModel;
	class CCollider;
}
class CSceneGate : public CGameObject
{
private:
	explicit CSceneGate(LPDIRECT3DDEVICE9 pGrpahicDev);
	virtual ~CSceneGate();

public:

	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CSceneGate* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();
	
	CTestTile* Set_GateDoor(bool bLeft);
	CTestTile* Set_GateStructure(const _vec3& vPos, const _vec3& vScale);

	bool Get_InGate() { return m_bInGate; };

private:
	CTransform* m_pTransform;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	wstring m_NextSceneTag;
	bool m_bInGate = false;
	bool m_bInOpendoor = false;

	CTestTile* pGateDoorL;
	CTestTile* pGateDoorR;
	CTestTile* pGatePillarL;
	CTestTile* pGatePillarR;
	CTestTile* pGateTop;

	CPlayerTriggerCube* pTriggerCube;

	vector<CGameObject*> m_vGameObjects;

	_float m_fDoorAngle = 0.f;
};

