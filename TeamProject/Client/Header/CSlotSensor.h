#pragma once
#include "CSensor.h"
#include "CTransform.h"
struct DetectZone {
	_vec3 _min;
	_vec3 _max;
};
class CMainPlayer;
class CSlotCube;
class CSlotCube_Auto;
class CSlotSensor: public CSensor
{

private:
	explicit CSlotSensor(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CSlotSensor(const CSlotSensor& rhs);
	virtual ~CSlotSensor();

public:
	//Basic Function
	HRESULT Ready_GameObject() override;
	_int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;

	//Create, Release Function
	static CSlotSensor* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

	//Core Function
	_bool Detect() override;

	//Getter, Setter Function
	void Set_Info(CMainPlayer* player, const _int puzzleID, const _int slotID);
	void Set_PuzzleID(const _int& ID) { m_iPuzzleID = ID; }
	void Set_SlottedCube(CSlotCube* pSlotCube) { m_pSlotted = pSlotCube; }
	void Set_SlottedCube_Auto(CSlotCube_Auto* pSlotCube) { m_pSlotted_Auto = pSlotCube; }
	void Set_PlayerPick(_bool bPick) { m_bPlayerPick = bPick; }

	const _vec3 Get_Look() { return m_pTransform->Get_Info(INFO_LOOK); }

private:
	//Compute Function
	void Insert_Slot();
	bool Check_Slot();

	//Variables
	DetectZone		m_Zone;
	_int			m_iPuzzleID;
	_int			m_iSlotID;
	CMainPlayer*	m_pPlayer;
	CSlotCube*		m_pSlotted;
	CSlotCube_Auto* m_pSlotted_Auto;

	CGameObject*	m_pPickObj = nullptr;
	CSlotCube*		m_pPickSlot = nullptr;
	CSlotCube_Auto* m_pPickSlot_Auto = nullptr;

	_bool m_bPlayerPick = true;
};

