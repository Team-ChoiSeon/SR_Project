#pragma once
#include "pch.h"
#include "SceneBG.h"
#include "CFactory.h"
#include "CFirstviewFollowingCamera.h"
#include "CMainPlayer.h"
#include "CCameraMgr.h"
#include "DummyCube.h"
SceneBG::SceneBG(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

SceneBG::~SceneBG()
{
}

HRESULT SceneBG::Ready_Scene()
{
	Init_Layers();
	CFactory::DeSerializeScene(L"../../Scene/SampleScene.json", this);

	// 4. 카메라 (플레이어 시점)
	CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);

	// 5. 플레이어 → 타겟 오브젝트
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	DummyCube* pPlayer = Get_Layer(LAYER_OBJECT)->Get_GameObject<DummyCube>(L"DummyCube_1");

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	// 6. 카메라 타겟은 플레이어
	pCam->Set_Target(pPlayer);  // 1인칭 시점
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);
	return S_OK;
}

_int SceneBG::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	return 0;
}

void SceneBG::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
}

SceneBG* SceneBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneBG* pScene = new SceneBG(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneBG::Free()
{
	CScene::Free();
}