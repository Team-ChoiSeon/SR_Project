#pragma once
#include "pch.h"
#include "SceneLoding.h"

#include "CTimeMgr.h"
#include "CInputMgr.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CResourceMgr.h"
#include "CPickingMgr.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CRenderMgr.h"

#include "CPlayer.h"
#include "CMainPlayer.h"
#include "CLightObject.h"
#include "CTestLightMeshObject.h"
#include "CCrosshairUIObject.h"
#include "DummyCube.h"
#include "CDirectionalCube.h"
#include "CTestTile.h"
#include "CSceneGate.h"

#include "SceneHW.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"


SceneLoding::SceneLoding(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pGraphicDev(pGraphicDev)
{

}

SceneLoding::~SceneLoding()
{
}

HRESULT SceneLoding::Ready_Scene()
{
	Init_Layers();

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int SceneLoding::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	return 0;
}

void SceneLoding::LateUpdate_Scene(const _float& fTimeDelta)
{

	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);

}

SceneLoding* SceneLoding::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneLoding* pScene = new SceneLoding(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneLoding::Free()
{
	Clear_Layers();
	CScene::Free();
}