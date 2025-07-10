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
	void	Render_Scene();

	//Create, Release Function
	static TestSceneHW* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Free();

private:
	//temp
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

	_float								m_fTime;
	bool						m_bCamPlayer;
};

