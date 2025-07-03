#pragma once
#include "pch.h"
#include "SceneSB.h"

#include "Player.h"
#include "DummyCube.h"
#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"
#include "CCameraMgr.h"
#include "CInputMgr.h"

SceneSB::SceneSB(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

SceneSB::~SceneSB()
{
}

HRESULT SceneSB::Ready_Scene()
{
	Init_Layers();
	Player* pPlayer = Player::Create(m_pGraphicDev);
	DummyCube* pDummy = DummyCube::Create(m_pGraphicDev);
	CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev, pPlayer);
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"MyPlayer", pPlayer);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"MyDummy", pDummy);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"FFCam", (pCam));

	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int SceneSB::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);

	return 0;
}

void SceneSB::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);

	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
}

void SceneSB::Render_Scene()
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Render_Layer();
}

SceneSB* SceneSB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneSB* pScene = new SceneSB(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneSB::Free()
{
	CScene::Free();
}