#pragma once
#include "pch.h"
#include "Logo.h" 

#include "CSceneMgr.h"
#include "CInputMgr.h"
#include "SceneBG.h"
#include "SceneDH.h"
#include "SceneHS.h"
#include "SceneHW.h"
#include "SceneSB.h"
#include "TestSceneHW.h"

#include "CSceneTestHS.h"
#include "SceneStage2.h"
#include "SceneLoding.h"

Logo::Logo(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

Logo::~Logo()
{
}

HRESULT Logo::Ready_Scene()
{
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
		CScene* pScene = SceneDH::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
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
		CScene* pScene = SceneSB::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F6))
	{
		//CScene* pScene = CSceneTestHS::Create(m_pGraphicDev);
		//CScene* pScene = SceneLoding::Create(m_pGraphicDev);
		CScene* pScene = SceneStage2::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F7))
	{
		CScene* pScene = TestSceneHW::Create(m_pGraphicDev);
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