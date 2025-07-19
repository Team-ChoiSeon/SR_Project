#pragma once
#include "CCube.h"

struct MetalZone {
	_vec3 _min;
	_vec3 _max;
};
enum class METAL_STATE { IDLE, APPROACH, SYNC, DETACH };
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
	void Set_Info(CMainPlayer* player);

	//Create, Release Function
	static CMetalCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	void DetectMagnetic(const _float& fTimeDelta);
	void ApproachtoMagnetic(const _float& fTimeDelta);
	void SyncMagnetic(const _float& fTimeDelta);
	void DetachMagnetic(const _float& fTimeDelta);

	//Getter, Setter Function

	METAL_STATE				m_eState;
private:

	//Variables
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;
	CMainPlayer* m_pPlayer;

	MetalZone	m_MZone;
	CGameObject*			m_pPickObj;
	CMagneticCube*			m_pPickMagnet;
	CGameObject*			m_pParentMagnet;

	_vec3					m_vParentPrePos;
	_vec3					m_vParentPos;
	_vec3					m_vGap;
	_vec3					m_vSyncGap;
};

