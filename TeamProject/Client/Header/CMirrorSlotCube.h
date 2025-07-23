#pragma once
#include "CSlotCube_Auto.h"

class CMainPlayer;

class CMirrorSlotCube : public CSlotCube_Auto
{
private:
	explicit CMirrorSlotCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CMirrorSlotCube(const CMirrorSlotCube& rhs);
	virtual ~CMirrorSlotCube();

public:
	//Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CMirrorSlotCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	// Follow Control
	void Set_Player(CMainPlayer* pPlayer) { m_pPlayer = pPlayer; }
	void Set_Follow(_bool bFollow);
	void Set_MirrorPlane(const _vec3& vPlanePos, const _vec3& vPlaneNormal);

	_bool Get_Follow() { return m_bFollow; }
private:
	void MirrorFollow(const _float& fTimeDelta);
	
private:
	CMainPlayer* m_pPlayer = nullptr;
	_bool m_bFollow = true;

	_vec3 m_vPlanePos = { 0.f, 0.f, 0.f };
	_vec3 m_vPlaneNorm = { 1.f, 0.f, 0.f }; // X√‡ ±‚¡ÿ 
	_vec3 m_vPrevPos = { 0.f,0.f,0.f };

	_float m_fMoveSpeed = 10.f;

	
};
