#pragma once
#include "CCube.h"

namespace Engine {
	class CCollider;
	class CRigidbody;
}
class CImpulseCube : public CCube
{
private: 
	explicit CImpulseCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CImpulseCube(const CImpulseCube& rhs);
	virtual ~CImpulseCube();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CImpulseCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Getter, Setter Function
	void Set_Info(const _vec3& StartPos);

private:
	//Compute Function

	//Variables
	CCollider* m_pCollider;
	CRigidbody* m_pRigid;
};

