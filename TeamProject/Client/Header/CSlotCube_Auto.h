#pragma once
#include "CCube.h"

namespace Engine
{
	class CCollider;
	class CRigidBody;
}
class CSlotSensor;
class CMainPlayer;

class CSlotCube_Auto : public CCube
{
private:
	explicit CSlotCube_Auto(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSlotCube_Auto(const CSlotCube_Auto& rhs);
	virtual ~CSlotCube_Auto();

public:
	//Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CSlotCube_Auto* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Getter, Setter Function
	void Set_Info(CMainPlayer* player, const _int puzzleID, const _int slotID);

	_int Get_PuzzleID() { return m_iPuzzleID; }
	_int Get_SlotID() { return m_iSlotID; }

	_bool Get_Slotted() { return m_bSlotted; }

	//Utility Function
	void Insert_Overlap(CSlotSensor* sensor, _float dist);

private:
	_bool Check_Overlap();
	void Fit(const _float& fTimeDelta);

private:
	CCollider* m_pCollider;
	CRigidBody* m_pRigid;

	CSlotSensor* m_FitSlot;
	unordered_map<CSlotSensor*, _float> m_vecDetected_Slot;
	_int		m_iPuzzleID;
	_int		m_iSlotID;
	_bool		m_bSlotted = false;
	CMainPlayer* m_pPlayer;
};
