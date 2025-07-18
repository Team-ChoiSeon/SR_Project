#pragma once
#include "CSwitch.h"
class CWeightSwitch : public CSwitch
{
private:
	explicit CWeightSwitch(LPDIRECT3DDEVICE9 CToggleSwitch);
	explicit CWeightSwitch(const CWeightSwitch& rhs);
	virtual ~CWeightSwitch();

public:
	// Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	// Create, Release Function
	static CWeightSwitch* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	// Getter, Setter Function
	void Set_Unreusable() { m_bReusable = false; }

private:
	// Compute Function
	void Init() override;

	// Variables

};

