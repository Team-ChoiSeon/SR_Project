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
class SceneLoding : public CScene
{
private:
	explicit SceneLoding(LPDIRECT3DDEVICE9 pGraphicDev);
public:
	virtual ~SceneLoding();

public:
	static SceneLoding* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
	virtual	HRESULT	Ready_Scene()override;
	virtual	_int	Update_Scene(const _float& fTimeDelta)override;
	virtual	void	LateUpdate_Scene(const _float& fTimeDelta)override;

	_float m_fProgress = 0.f;
	_float m_fProgressTimer = 0.f;

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;

public:
	virtual void Free();
};
