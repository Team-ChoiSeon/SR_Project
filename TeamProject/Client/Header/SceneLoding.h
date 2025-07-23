#pragma once
#include "CScene.h"

class CLightObject;
class CTestLightMeshObject;
class DummyCube;
class CFirstviewFollowingCamera;
class CCrosshairUIObject;
class CMainPlayer;
class CTestTile;

namespace Engine {
	class CScene;
	class CLayer;
}

class SceneLoding : public CScene
{
	enum class LOADING_STEP {NONE, READ_SCENE, LOAD_LAYER,LOAD_OBJ,CHANGE_SCENE};
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
	virtual void Set_Cam();

	virtual HRESULT LoadScene(CScene* from, CScene* to); //1
private:
	void	Load_Layer(); //2
	void LoadObject(); //3
	virtual void Change_Scene(); //4

private:
	_uint m_fProgress = 0;
	_uint m_fMaxProgress = 0;
	_float m_fProgressTimer = 0.f;

private:
	LPDIRECT3DDEVICE9	m_pGraphicDev;
	LOADING_STEP m_eNowStep;

	json jLayers;
	json jObjects;

	size_t layerCount;
	size_t ObjectsCount;

	json::iterator currentLayerIter;
	json::iterator currentObjectIter;

	CScene* m_pFrom = nullptr;
	CScene* m_pTo  = nullptr;

	CLayer* m_ReadingLayer;

	_vec3 DummyPos;
	CFirstviewFollowingCamera* m_pCam;
	CTestTile* m_pTarget;
public:
	virtual void Free();
};
