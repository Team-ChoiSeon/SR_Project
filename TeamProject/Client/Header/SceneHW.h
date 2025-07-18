#pragma once
#include "CScene.h"

class CMainPlayer;
class DummyCube;
class CFirstviewFollowingCamera;
class CFloatingCube;
class CDirectionalCube;
class CImpulseCube;
class CPlayer;
class CTestTile;
class CPickSwitch;
class CSlotCube;
class CSlotSensor;
class CMagneticCube;
class CMetalCube;
class CLowGravityCube;

class SceneHW : public CScene
{
private:
	explicit SceneHW(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~SceneHW();

	HRESULT Ready_Scene();

public:
	//Basic Function
	int		Update_Scene(const _float& fTimeDelta);
	void	LateUpdate_Scene(const _float& fTimeDelta);

	//Create, Release Function
	static SceneHW* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Free();


	virtual const wstring Get_ScenePath() { return L"../../Scene/empty.json"; }

private:
	//temp
	//CPlayer*							m_pPlayer;
	CMainPlayer*						m_pPlayer;
	CFirstviewFollowingCamera*			m_pFFCam;
	CFirstviewFollowingCamera*			m_pdummycam;
	DummyCube*							m_pDummy;
	CFloatingCube* m_pFloatingCube;
	CFloatingCube* m_pFloatingCube2;
	CDirectionalCube*					m_pDirectionalCube;
	CDirectionalCube*					m_pOnewayCube;
	CImpulseCube*						m_pImpulseCube;
	CTestTile*							m_pTile;
	CPickSwitch*						m_pPickSwitch;
	CSlotCube*							m_pSlotCube;
	CSlotCube*							m_pSlotCube2;
	CSlotCube*							m_pSlotCube3;
	CSlotSensor*						m_pSlotSensor;
	CSlotSensor*						m_pSlotSensor2;
	CSlotSensor* m_pSlotSensor3;
	CMagneticCube*						m_pTestmagnet;
	CMetalCube*						m_pTestmetal;
	CLowGravityCube* m_pLowGravityCube;
	_float								m_fTime;
	bool								m_bCamPlayer;
	_bool								m_bSceneFirst;
};

