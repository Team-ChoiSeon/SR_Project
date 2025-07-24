#pragma once
#include "pch.h"
#include "Engine_Define.h"
#include "Logo.h" 
#include "CGameObject.h" 

#include "CSceneMgr.h"
#include "CInputMgr.h"
#include "SceneBG.h"
#include "SceneHS.h"
#include "SceneHW.h"
#include "SceneSB.h"
#include "TestSceneHW.h"
#include "BossScene.h"

#include "CSceneTestHS.h"
#include "SceneStage1.h"
#include "SceneStage2.h"
#include "SceneStage3.h"
#include "SceneLoding.h"

#include "CMainPlayer.h"
#include "CTestTile.h"
#include "CFirstviewFollowingCamera.h"
#include "CLogoDirector.h"

#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CTransform.h"

Logo::Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

Logo::~Logo()
{
}

HRESULT Logo::Ready_Scene()
{
	Init_Layers();

	CMainPlayer* pPlayer = CMainPlayer::Create(m_pGraphicDev);
	pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 5.f, -50.f });
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);

	CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	pCam->Set_Target(pPlayer);


	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	// 4. 마지막으로 로고 연출을 담당할 디렉터를 생성합니다.
	CLogoDirector* pDirector = CLogoDirector::Create(m_pGraphicDev);

	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"DummyPlayer", pPlayer);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MainCamera", pCam);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"LogoDirector", pDirector);
	return S_OK;
}

_int Logo::Update_Scene(const _float& fTimeDelta)
{
	CScene::Update_Scene(fTimeDelta);


	if (CInputMgr::Get_Instance()->Key_Tap(DIK_F1))
	{
		CScene* pScene = SceneBG::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if(CInputMgr::Get_Instance()->Key_Tap(DIK_F2))
	{
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F3))
	{
		CScene* pScene = SceneHS::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F4))
	{
		CScene* pScene = SceneHW::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F5))
	{
		CScene* pScene = BossScene::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F6))
	{
		//CScene* pScene = CSceneTestHS::Create(m_pGraphicDev);
		//CScene* pScene = SceneLoding::Create(m_pGraphicDev);
		CScene* pScene = SceneStage3::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F7))
	{
		CScene* pScene = SceneStage1::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	
	return 0;
}

void Logo::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}


Logo* Logo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	Logo* pScene = new Logo(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void Logo::Free()
{
	CScene::Free();
}