#pragma once
#include "CCube.h"

struct MetalZone {
	_vec3 _min;
	_vec3 _max;
};

namespace Engine {
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CMagneticCube;
class CMainPlayer;

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

	void GluetoMagnetic(CMagneticCube* parentMagnet, const _float& fTimeDelta);
	void DetectMagnet(const _float& fTimeDelta);
	//Getter, Setter Function

private:

	//Variables
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;
	CMainPlayer* m_pPlayer;

	MetalZone	m_MZone;
	CGameObject* m_pPickObj;
	CMagneticCube* m_pPickMagnet;
	CMagneticCube* m_pParentMagnet;
	vector<CMagneticCube*> m_vecDetectedMagnets;
};

