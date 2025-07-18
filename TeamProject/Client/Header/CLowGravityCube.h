#pragma once
#include "CCube.h"

namespace Engine
{
	class CCollider;
	class CRigidBody;
}
class CLowGravityCube : public CCube
{
private:
	explicit CLowGravityCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLowGravityCube(const CLowGravityCube& rhs);
	virtual ~CLowGravityCube();
	
public:
	//Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, ReleaseFunction
	static CLowGravityCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Getter, Setter Function


private:
	//Compute Function
	void Detect();
	void Push();
	void Restoration();

	//Variables
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	CGameObject* m_pColTarget;
	CGameObject* m_pPreColTarget;

	_bool m_bFirstCol = true;
	_float m_fColGravity;

};

