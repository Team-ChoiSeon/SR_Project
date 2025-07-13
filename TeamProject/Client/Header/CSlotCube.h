#pragma once
#include "CCube.h"

namespace Engine
{
	class CCollider;
	class CRigidBody;
	class CPickTarget;
}
class CSlotSensor;
class CMainPlayer;
class CSlotCube : public CCube
{
private:
	explicit CSlotCube(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSlotCube(const CSlotCube& rhs);
	virtual ~CSlotCube();

public:
	//Basic Function
	HRESULT Ready_GameObject()override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CSlotCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Getter, Setter Function
	void Set_Info(CMainPlayer* player, const _int ID, const _int Seuqence);


	_int Get_PuzzleID() { return m_iPuzzleID; }
	_int Get_SlotID() { return m_iSlotID; }
	_bool Get_Lay() { return m_bLay; }

	//Utility Function
	void Insert_Overlap(CSlotSensor* sensor, _float dist);

private:
	//Compute Function
	_bool Check_Overlap();
	void PickMove();
	void Fit(const _float& fTimeDelta);
	//Variables
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;
	CPickTarget* m_pPick;

	CSlotSensor* m_FitSlot;
	unordered_map<CSlotSensor*, _float> m_vecDetected_Slot;
	_int		m_iPuzzleID;
	_int		m_iSlotID;
	_bool		m_bSlotted;
	CMainPlayer* m_pPlayer;
	_bool		m_bFirstPick;

	_vec3		m_vDist;
	_float		m_fDist;
};

