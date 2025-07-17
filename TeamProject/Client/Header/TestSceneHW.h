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

	virtual const wstring Get_ScenePath() { return L"../../Scene/Stage_hw_1.json"; }
private:
	//temp
	void FloatingSet();
	void DirectionSet();
	void SlotSet();

	CMainPlayer*						m_pPlayer;
	CFirstviewFollowingCamera*			m_pFFCam;

	bool						m_bSceneFirst =true;
};

