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
#include "CFirstviewFollowingCamera.h"
#include "CLogoDirector.h"
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
	if (pPlayer)
	{
		// 카메라를 로고가 잘 보일 만한 거리로 미리 이동시킵니다.
		pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 5.f, -50.f });
		Get_Layer(LAYER_PLAYER)->Add_GameObject(L"DummyPlayer", pPlayer);

		// 3. 플레이어를 따라다니는 1인칭 카메라를 생성하고 부착합니다.
		CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
		if (pCam)
		{
			pCam->Set_Target(pPlayer);
			Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MainCamera", pCam);
		}
	}

	// 4. 마지막으로 로고 연출을 담당할 디렉터를 생성합니다.
	CGameObject* pDirector = CLogoDirector::Create(m_pGraphicDev);
	if (pDirector)
	{
		Get_Layer(LAYER_OBJECT)->Add_GameObject(L"LogoDirector", pDirector);
	}

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