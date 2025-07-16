#pragma once
#include "CGameObject.h"
#include "CMonsterPart.h"

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

	void On_Hit(const _vec3& hitpos);
	void Key_Input(const _float& fTimeDelta);

	static CVellum* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free();

public:
	CTransform*		Get_HTransform()	{ return m_pTransform; }
	CRigidBody*		Get_HRigid()		{ return m_pRigid; }
	CCollider*		Get_HCol()			{ return m_pCol; }
	
	CGameObject*	Get_Target()		{ return m_pTarget; }
	const vector<CMonsterPart*>& Get_Part() const { return m_vPart; }

public:
	void Change_Pattern(IVellumState* pState);

private:
	int m_iHP = 100;			// 체력
	_float m_fInvTime = 1.f;	// 무적 타이머
	int m_iPartCnt = 10;			// 머리 제외 파츠 개수
	vector<CMonsterPart*> m_vPart;

	IVellumState* m_pState = nullptr;

	// 플레이어 정보
	CGameObject* m_pTarget = nullptr;

	// 헤드정보
	CModel*		m_pModel = nullptr;
	CTransform* m_pTransform = nullptr;
	CRigidBody* m_pRigid = nullptr;
	CCollider*	m_pCol = nullptr;



};

