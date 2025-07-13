#pragma once
#include "CGameObject.h"

namespace Engine {
	class CModel;
	class CTransform;
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
	
	void Set_CursorVec(const _vec3& cursorvec) { m_vCursorDelta = cursorvec; }
	void Set_Grab(bool Trigger) { m_bCurGrab = Trigger; }

protected:
	//Compute Function
	void Check_Lay();
	//Variables
	CModel* m_pModel;
	CTransform* m_pTransform;

	_vec3			m_vCursorDelta;
	_bool			m_bCurGrab = false;
	_bool			m_bPreGrab = false;
	_bool			m_bLay = false;
};

