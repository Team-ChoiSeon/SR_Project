#pragma once
#include "CScene.h"

class CMainPlayer;
class DummyCube;
class CFirstviewFollowingCamera;
class CFloatingCube;
class CDirectionalCube;
class CImpulseCube;
class CWeightButton;
class CPlayer;
class CTestTile;
class CTimerButton;

class TestSceneHW : public CScene
{
private:
	explicit TestSceneHW(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~TestSceneHW();

	HRESULT Ready_Scene();

public:
	//Basic Function
	int		Update_Scene(const _float& fTimeDelta);
	void	LateUpdate_Scene(const _float& fTimeDelta);

	//Create, Release Function
	static TestSceneHW* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Free();

	virtual const wstring Get_ScenePath() { return L"../../Scene/hwtest_2.json"; }
private:
	//temp
	void FloatingSet();
	void DirectionSet();
	void SlotSet();
	//CPlayer*							m_pPlayer;
	CMainPlayer* m_pPlayer;
	CFirstviewFollowingCamera* m_pFFCam;
	CFirstviewFollowingCamera* m_pdummycam;
	DummyCube* m_pDummy;
	CFloatingCube* m_pFloatingCube;
	CDirectionalCube* m_pDirectionalCube;
	CDirectionalCube* m_pOnewayCube;
	CImpulseCube* m_pImpulseCube;
	CWeightButton* m_pWeightButton;
	CTestTile* m_pTile;
	CTimerButton* m_pTimerButton;
	CSlotCube* m_pSlotCube;
	_float								m_fTime;
	bool						m_bCamPlayer;

	bool						m_bSceneFirst =true;
};

