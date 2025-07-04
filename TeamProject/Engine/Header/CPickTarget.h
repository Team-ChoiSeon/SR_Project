#pragma once
#include "CComponent.h"

class CPickTarget : public CComponent
{
private:
	explicit CPickTarget();
	explicit CPickTarget(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype);
	explicit CPickTarget(const CPickTarget& rhs);
	virtual ~CPickTarget();

public:
	//Basic Function
	HRESULT Ready_Component();
	void	Update_Component(const float& fTimeDelta);
	void	LateUpdate_Component(const float& fTimeDelta);


	//Create, Release Function
	static CPickTarget* Create(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype);
	void				Free();

	//Core Function
	void Check_Collision();
	void Add_HitInfo();

	//Optional Function
	
	//Getter,Setter Function


private:
	//Variables
	HWND					m_HWND;
	_vec2					m_vCursorPos;
	RAYCHECKTYPE			m_eRayCheckType;
	Ray_Hit					m_pRayHitInfo;
	POINT					m_pCursr;

};

