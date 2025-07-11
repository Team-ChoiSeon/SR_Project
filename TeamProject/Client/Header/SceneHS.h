#pragma once
#include "CScene.h"

class CLightObject;
class CTestLightMeshObject;
class DummyCube;
class CFirstviewFollowingCamera;
class CCrosshairUIObject;
class CMainPlayer;

namespace Engine {
	class CLayer;
}
class SceneHS : public CScene
{
private:
	explicit SceneHS(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneHS();

public:
	static SceneHS* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual	HRESULT	Ready_Scene()override;
	virtual	_int	Update_Scene(const _float& fTimeDelta)override;
	virtual	void	LateUpdate_Scene(const _float& fTimeDelta)override;
	void	Render_Scene();

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	virtual void Free();
};

