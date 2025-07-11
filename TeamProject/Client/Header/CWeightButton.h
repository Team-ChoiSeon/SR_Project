#pragma once
#include "CActiveTrigger.h"

namespace Engine
{
	class CTransform;
	class CModel;
	class CCollider;
	class CRigidBody;
}
class CWeightButton : public CActiveTrigger
{
private:
	explicit CWeightButton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CWeightButton(const CWeightButton& rhs);
	virtual ~CWeightButton();

public:
	// Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	// Create, Release Function
	static CWeightButton* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	// Getter, Setter Function
	void Set_Info(const _vec3& startpos, const _float& weight, const _float& recyclecount);
	void Set_Trigger(const _float& weight);

private:
	// Compute Function
	void Checkcollision();

	// Variables
	CTransform*				m_pTransform;
	CModel*					m_pModel;
	CCollider*				m_pCollider;
	CRigidBody*				m_pRigid;
};

