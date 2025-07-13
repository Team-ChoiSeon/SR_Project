#pragma once
#include "CCube.h"
#include "CSlotSensor.h"

enum class MAGNETTYPE { N, S, METAL, NONE };
namespace Engine {
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CMainPlayer;
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
	void MagneticMove();
	void DetectMagnetic();

	//Variables
	CCollider*			m_pCollider;
	CRigidBody*			m_pRigid;
	CPickTarget*		m_pPick;

	CMagneticCube*		m_pParentMag;
	CMainPlayer*		m_pPlayer;

	_vec3				m_vDist;
	_float				m_fDist;
	_float				m_fDetectDist;
	_bool				m_bParent;
	MAGNETTYPE			m_eMagnetType = MAGNETTYPE::METAL;
	DetectZone			m_Zone;

	vector<CMagneticCube*>	m_vecNearMag;
};

