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

class CToggleButton : public CActiveTrigger
{
private:
	explicit CToggleButton(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CToggleButton(const CToggleButton& rhs);
	virtual ~CToggleButton();

public:
	// Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	// Create, Release Function
	static CToggleButton* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	// Getter, Setter Function
	void Set_Reusable() { m_bReusable = true; }

private:
	// Compute Function

	// Variables
	CTransform*			m_pTransform;
	CModel*				m_pModel;
	CCollider*			m_pCollider;
	CRigidBody*			m_pRigid;
	CPickTarget*		m_pPick;

	_bool				m_bReusable = false;
	_bool				m_bPressed = false;
};

