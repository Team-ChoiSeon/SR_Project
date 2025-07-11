#pragma once
#include "CSwitch.h"
namespace Engine
{
	class CTransform;
	class CModel;
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}

class CPickSwitch : public CSwitch
{
private:
	explicit CPickSwitch(LPDIRECT3DDEVICE9 CToggleSwitch);
	explicit CPickSwitch(const CPickSwitch& rhs);
	virtual ~CPickSwitch();

public:
	// Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	// Create, Release Function
	static CPickSwitch* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	// Getter, Setter Function
	void Set_Unreusable() { m_bReusable = false; }

private:
	// Compute Function
	void Init() override;

	// Variables
};

