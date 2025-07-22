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
	void Set_Follow(bool bFollow) { m_bFollow = bFollow; }
	void Set_InvertX(bool bInvert) { m_bInvertX = bInvert; }
	void Set_InvertZ(bool bInvert) { m_bInvertZ = bInvert; }

private:
	void MirrorFollow(const _float& fTimeDelta);

private:
	CMainPlayer* m_pPlayer = nullptr;
	bool m_bFollow = true;
	bool m_bInvertX = true;
	bool m_bInvertZ = true;

};
