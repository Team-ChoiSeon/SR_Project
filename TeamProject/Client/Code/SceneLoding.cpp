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
#include "CProgressBar.h"
#include "CLodingCube.h"

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

	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", CMainPlayer::Create(m_pGraphicDev));
	//Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"Player")->Get_Component<CTransform>()->Set_Pos({ -20.f, -20.f, -20.f });
	CSceneMgr::Get_Instance()->Set_Player(Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"Player"));

	CProgressBar* pProgressBar = CProgressBar::Create(m_pGraphicDev);
	pProgressBar->Set_Progress(0.f);
	Get_Layer(LAYER_UI)->Add_GameObject(L"ProgressBar", pProgressBar);

	CLodingCube* m_pRotateCube = CLodingCube::Create(m_pGraphicDev);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"RotateCube", m_pRotateCube);

	CTestTile* m_pCamTargetDummy = CTestTile::Create(m_pGraphicDev);
	_vec3 vCubePos = m_pRotateCube->Get_Component<CTransform>()->Get_Pos();
	_vec3 vDummyPos = vCubePos + _vec3(-1.38f, -0.12f, -3.3f);
	//_vec3 vDummyPos = vCubePos + _vec3(-0.7f, 0.85f, -3.f);
	m_pCamTargetDummy->Get_Component<CTransform>()->Set_Pos(vDummyPos);
	m_pCamTargetDummy->Get_Component<CTransform>()->Set_ScaleY(2.f);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"CamTargetDummy", m_pCamTargetDummy);


	CFirstviewFollowingCamera* pDummyCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	pDummyCam->Set_Target(m_pCamTargetDummy);

	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"DummyCam", pDummyCam);
	CCameraMgr::Get_Instance()->Set_MainCamera(pDummyCam);

	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"ProgressBar"));

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int SceneLoding::Update_Scene(const _float& fTimeDelta)
{
	CScene::Update_Scene(fTimeDelta);

	m_fProgressTimer += fTimeDelta;

	if (m_fProgress < 1.f && m_fProgressTimer >= .2f)
	{
		m_fProgress += 0.01f;
		m_fProgress = min(m_fProgress, 1.f);
		Get_Layer(LAYER_UI)->Get_GameObject<CProgressBar>(L"ProgressBar")->Set_Progress(m_fProgress);
		m_fProgressTimer = 0.f;
	}


	return 0;
}

void SceneLoding::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);

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
	CScene::Free();
}