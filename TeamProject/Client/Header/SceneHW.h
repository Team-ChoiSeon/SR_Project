#pragma once
#include "CScene.h"

class CMainPlayer;
class DummyCube;
class CFirstviewFollowingCamera;

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
	void	Render_Scene();

	//Create, Release Function
	static SceneHW* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void	Free();

private:
	CMainPlayer*						m_pPlayer;
	CFirstviewFollowingCamera*	m_pFFCam;
	CFirstviewFollowingCamera*	m_pdummycam;
	DummyCube*					m_pDummy;

	bool						m_bCamPlayer;
};

