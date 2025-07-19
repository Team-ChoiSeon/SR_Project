#pragma once
#include "CGameObject.h"

namespace Engine
{
	class CTransform;
	class CModel;
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}

class CSensor : public CGameObject
{
protected:
	explicit CSensor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSensor(const CSensor& rhs);
	virtual ~CSensor();

public:
	// Basic Function
	virtual HRESULT Ready_GameObject() override = 0;
	virtual _int Update_GameObject(const _float& fTimeDelta) override = 0;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override = 0;

	// Create, Release Function
	virtual void Free() override = 0;

	// Getter, Setter Function
	const _bool Get_SensorState() const { return m_bSensorOn; }

	// Compute Function
	virtual  _bool Detect() = 0;

protected:
	// Variables
	_bool			m_bSensorOn;
	CGameObject*	m_pDetectedGameObject;

	CTransform*		m_pTransform;
	CCollider*		m_pCollider;
	CRigidBody*		m_pRigid;
};

