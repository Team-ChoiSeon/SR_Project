#pragma once
#include "CScene.h"

class Player;
class DummyCube;
class CFirstviewFollowingCamera;

namespace Engine {
	class CLayer;
}
class SceneHW : public CScene
{
public:
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
	LPDIRECT3DDEVICE9			m_pGraphicDev;
	CLayer*						m_pPlayerLayer;
	CLayer*						m_pObjectLayer;
	CLayer*						m_pCameraLayer;
	Player*						m_pPlayer;
	ID3DXFont*					m_pFont;
	CFirstviewFollowingCamera*	m_pFFCam;
	CFirstviewFollowingCamera*	m_pdummycam;
	DummyCube*					m_pDummy;

	bool						m_bCamPlayer;
};

