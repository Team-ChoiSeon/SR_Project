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

class CSwitch : public CGameObject
{
protected:
	explicit CSwitch(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSwitch(const CSwitch& rhs);
	virtual ~CSwitch();

public:
	// Basic Function
	HRESULT Ready_GameObject() override = 0;
	_int Update_GameObject(const _float& fTimeDelta) override = 0;
	void LateUpdate_GameObject(const _float& fTimeDelta) override = 0;

	// Create, Release Function
	void Free() override = 0;

	// Getter, Setter Function
	const _bool Get_SwitchState() const { return m_bButtonOn; }
	void Set_Button() { m_bReusable = false; }
	void Set_Grab(const _bool grab) { m_bGrab = grab; }
	void Set_Tap(bool Trigger) { m_bTap = Trigger; }
	void Set_Away(bool Trigger) { m_bAway = Trigger; }
	void Set_CursorVec(const _vec3& cursorvec) { m_vCursorDelta = cursorvec; }

	_bool	Get_Grab() { return m_bGrab; }
	_bool	Get_Tap() { return m_bTap; }
	_bool	Get_Away() { return  m_bAway; }

protected:
	// Compute Function
	virtual void Init() = 0;
	virtual void SwitchOn(bool trigger);
	virtual void Pressing(const _float& fTimeDelta);

	// Variables
	CTransform*			m_pTransform;
	CModel*				m_pModel;
	CCollider*			m_pCollider;
	CRigidBody*			m_pRigid;
	CPickTarget*		m_pPick;

	_bool				m_bButtonOn = false;
	_bool				m_bInit = false;
	_bool				m_bReusable = true;
	_bool				m_bPressed = false;
	_bool				m_bGrab = false;
	_bool				m_bTap = false;
	_bool				m_bAway = false;

	_vec3				m_vCursorDelta;
	_vec3				m_vOriginalScale;
	_vec3				m_vOriginalPos;


};

