#pragma once
#include "CGameObject.h"

#include "CTransform.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidBody.h"

enum class ETileState { IDLE, DESTROYING, DESTROYED, RESTORING};

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

	void Set_Destroy(bool bDestroy);



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

	ETileState m_eState = ETileState::IDLE;
	float m_fEDuration = 2.f;	// ¿Ã∆Â∆Æ
	float m_fETimer = 0.f;   
	float m_fRDuration = 5.f;   // ∫π±∏
	float m_fRTimer = 0.f;  
};