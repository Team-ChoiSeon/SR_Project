#pragma once
#include "CComponent.h"
#include "CCollider.h"

BEGIN(Engine)
//class CCollider;
class ENGINE_DLL CPickTarget : public CComponent
{
private:
	explicit CPickTarget();
	explicit CPickTarget(LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype);
	explicit CPickTarget(const CPickTarget& rhs);
	virtual ~CPickTarget();

public:
	//Basic Function
	HRESULT Ready_Component();
	void	Update_Component(const float& fTimeDelta);
	void	LateUpdate_Component(const float& fTimeDelta);


	//Create, Release Function
	static CPickTarget* Create(LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype);
	void				Free();

	//Core Function
	bool Check_Collision();
	void Add_HitInfo();

	//Optional Function
	
	//Getter,Setter Function
	void Set_RayCheckType(RAYCHECKTYPE eRayCheckType) { m_eRayCheckType = eRayCheckType; }
	//void Set_RayBoundTri(Ray_Tri* boundtri) { m_pRayTri = boundtri; }

private:
	//Compute Function
	void TranslationIntoLocal();
	bool CheckAABB();
	bool CheckOBB();
	bool CheckSPHERE();
	bool CheckTRI();

	//Variables
	Ray*					m_pRay;
	RAYCHECKTYPE			m_eRayCheckType;
	Ray_Hit					m_RayHitInfo;
	CCollider*				m_pCollider = nullptr;

};

END
