#pragma once
#include "CScene.h"

class Player;
class DummyCube;
class CFirstviewFollowingCamera;
class SceneHW : public CScene
{
public:
	explicit SceneHW(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~SceneHW();

	HRESULT Ready_Scene();

public:
	static SceneHW* Create(LPDIRECT3DDEVICE9 pGraphicDev);

	int		Update_Scene(const _float& fTimeDelta);
	void	LateUpdate_Scene(const _float& fTimeDelta);
	void	Render_Scene();

	void	Free();

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	Player*				m_pPlayer;
	ID3DXFont*			m_pFont;
	CFirstviewFollowingCamera* m_pFFcam;
	DummyCube*			m_pDummy;
};

