#pragma once
#include "CCube.h"

struct MagnetZone {
	_vec3 _min;
	_vec3 _max;
};
namespace Engine {
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CMainPlayer;
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
	void DetectMetal();

	//Variables
	CCollider*			m_pCollider;
	CCollider*			m_pDetectCollider;
	CRigidBody*			m_pRigid;
	CPickTarget*		m_pPick;

	CMainPlayer*		m_pPlayer;

	_vec3				m_vDist;
	_float				m_fDist;
	_float				m_fDetectDist = 3.f;
	_bool				m_bParent;

	MagnetZone			m_MZone;
	vector<CMetalCube*>	m_vecMetal;
	vector<CMetalCube*> m_vecCloseMetal;
};

