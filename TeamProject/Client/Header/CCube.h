#pragma once
#include "CGameObject.h"

namespace Engine {
	class CModel;
	class CTransform;
	class CRigidBody;
	class CCollider;
}
class CCube : public CGameObject
{
protected:
	explicit CCube(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CCube();

public:
	//Basic Function
	virtual HRESULT Ready_GameObject() override = 0;
	virtual _int Update_GameObject(const _float& fTimeDelta) override = 0;
	virtual void LateUpdate_GameObject(const _float& fTimeDelta) override = 0;

	//Create, Release Function
	virtual void Free() = 0;

	//Getter, Setter Function
	_bool	Get_Grab() { return m_bCurGrab; }
	_bool	Get_Tap() { return m_bTap; }
	_bool	Get_Away() { return  m_bAway; }
	
	void Set_CursorVec(const _vec3& cursorvec) { m_vCursorDelta = cursorvec; }
	void Set_Grab(bool Trigger) { m_bCurGrab = Trigger; }
	void Set_Tap(bool Trigger);
	void Set_Away(bool Trigger);

protected:
	//Compute Function
	//Variables
	CModel* m_pModel;
	CTransform* m_pTransform;
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	_vec3			m_vCursorDelta;
	_bool			m_bCurGrab = false;
	_bool			m_bPreGrab = false;
	_bool			m_bTap = false;
	_bool			m_bAway = false;
	_bool			m_bLay = false;
};

