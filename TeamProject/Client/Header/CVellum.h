#pragma once
#include "CGameObject.h"
#include "CMonsterPart.h"

#include "CModel.h"

#include "IVellumState.h"




class CVellum : public CGameObject
{
private:
	explicit CVellum(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CVellum();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	void Key_Input(const _float& fTimeDelta);

	static CVellum* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

public:
	CMonsterPart*	Get_Head()			{ return m_vPart[0]; }
	CTransform*		Get_HTransform()	{ return m_pTransform; }
	CRigidBody*		Get_HRigid()		{ return m_pRigid; }
	CCollider*		Get_HCol()			{ return m_pCol; }
	
	CGameObject*	Get_Target()		{ return m_pTarget; }
	const vector<CMonsterPart*> Get_Part() { return m_vPart; }

public:
	void Change_Pattern(IVellumState* pState);

private:
	int m_iPartCnt = 7;
	vector<CMonsterPart*> m_vPart;

	IVellumState* m_pState = nullptr;

	// 플레이어 정보
	CGameObject* m_pTarget = nullptr;

	// 헤드정보
	CTransform* m_pTransform = nullptr;
	CRigidBody* m_pRigid = nullptr;
	CCollider*	m_pCol = nullptr;



};

