#pragma once
#include "CScene.h"

class CLightObject;
class CTestLightMeshObject;
class DummyCube;
class CFirstviewFollowingCamera;
class CCrosshairUIObject;

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
	CLayer*				m_pObjectLayer = nullptr;
	CLayer*				m_pCameraLayer = nullptr;

	ID3DXFont*			m_pFont = nullptr;

	CLightObject*		m_pLightObject = nullptr;
	CTestLightMeshObject* m_pTestLightMesh = nullptr;
	//CFirstviewFollowingCamera* m_pFFCam;
	CFirstviewFollowingCamera* m_pdummycam = nullptr;
	DummyCube*			m_pDummy = nullptr;
	CCrosshairUIObject* m_pCrosshair = nullptr;

public:
	virtual void Free();
};

