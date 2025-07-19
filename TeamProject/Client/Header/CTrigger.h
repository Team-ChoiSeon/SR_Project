#pragma once
#include "CGameObject.h"

class CTrigger : public CGameObject
{
protected:
	explicit CTrigger(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CTrigger(const CTrigger& rhs);
	virtual ~CTrigger();

public:
	// Basic Function
	virtual HRESULT Ready_GameObject() override = 0;
	virtual _int Update_GameObject(const _float& fTimeDelta) override = 0;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override = 0;

	// Create, Release Function
	virtual void Free() override =0;

	// Getter, Setter Function
	const _bool Get_TriggerState() const { return m_bTriggerOn; }
protected:
	// Compute Function

	// Variables
	_bool m_bTriggerOn;
};

