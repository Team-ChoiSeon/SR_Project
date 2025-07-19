#pragma once
#include "CScene.h"

class CLightObject;
class CTestLightMeshObject;
class DummyCube;
class CFirstviewFollowingCamera;
class CCrosshairUIObject;
class CMainPlayer;


class SceneHS : public CScene
{
private:
	explicit SceneHS(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneHS();

public:
	virtual	HRESULT	Ready_Scene()override;
	virtual	_int	Update_Scene(const _float& fTimeDelta)override;
	virtual	void	LateUpdate_Scene(const _float& fTimeDelta)override;

	void End_Cinematic();
public:
	static SceneHS* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual void Free();

	virtual const wstring Get_ScenePath() { return L"../../Scene/empty.json"; }
};

