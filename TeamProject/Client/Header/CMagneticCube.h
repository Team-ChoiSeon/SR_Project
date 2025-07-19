#pragma once
#include "CCube.h"

namespace Engine {
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CMetalCube;
class CMagneticCube : public CCube
{
private:
	explicit CMagneticCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMagneticCube(const CMagneticCube& rhs);
	virtual ~CMagneticCube();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Set_Info();

	//Create, Release Function
	static CMagneticCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Getter, Setter Function

private:
	void PickMove();

	//Variables
	CCollider*			m_pCollider;
	CCollider*			m_pDetectCollider;
	CRigidBody*			m_pRigid;
	CPickTarget*		m_pPick;
};

