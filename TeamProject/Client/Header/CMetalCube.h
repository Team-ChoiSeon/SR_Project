#pragma once
#include "CCube.h"



namespace Engine {
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CMagneticCube;

class CMetalCube : public CCube
{
private:
	explicit CMetalCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMetalCube(const CMetalCube& rhs);
	virtual ~CMetalCube();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Set_Info();

	//Create, Release Function
	static CMetalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	void MovetoMagnetic(CMagneticCube* parentMagnet);
	//Getter, Setter Function

private:

	//Variables
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	CMagneticCube* m_pParentMagnet;
};

