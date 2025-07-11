#pragma once
#include "CGameObject.h"

#include "CTransform.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidBody.h"


class CTestTile : public Engine::CGameObject
{
private:
	CTestTile(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CTestTile();

public:
	//Basic Function
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CTestTile* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();



protected:

private:
	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;


	const _matrix* m_mWorld;


	float m_fWidth;
	float m_fDepth;

	bool m_bCursorMove;
};