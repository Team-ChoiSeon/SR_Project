#pragma once
#include "CActiveTrigger.h"

namespace Engine
{
	class CTransform;
	class CModel;
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}

class CTimerButton : public CActiveTrigger
{
private:
	explicit CTimerButton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTimerButton(const CTimerButton& rhs);
	virtual ~CTimerButton();

public:
	// Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	// Create, Release Function
	static CTimerButton* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	// Getter, Setter Function
	void Set_Info(const _vec3& startpos, const _float& timelimit, const _float& recyclecount);
	void Set_Trigger(_float& time);

private:
	// Compute Function
	void CheckActive();

	// Variables
	CTransform* m_pTransform;
	CModel* m_pModel;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;
	CPickTarget* m_pPick;

	_float	m_fTime;

};

