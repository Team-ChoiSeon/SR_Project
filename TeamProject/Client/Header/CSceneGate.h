#pragma once
#include "CGameObject.h"
#include "CCollider.h"
#include "CRigidBody.h"

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
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	static CSceneGate* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();
	
	bool Get_InGate() { return m_bInGate; };

private:
	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	wstring m_NextSceneTag;
	bool m_bInGate = false;
};

