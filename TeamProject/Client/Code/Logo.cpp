#pragma once
#include "pch.h"
#include "Engine_Define.h"
#include "Logo.h" 
#include "CGameObject.h" 

#include "CSceneMgr.h"
#include "CInputMgr.h"

#include "CRenderMgr.h"
#include "CPostProcess.h"

#include "SceneBG.h"
/*
#include "SceneHS.h"
#include "SceneHW.h"
#include "SceneSB.h"*/

#include "CSceneTestHS.h"
#include "BossScene.h"
#include "SceneStage1.h"
#include "SceneStage2.h"
#include "SceneStage3.h"
#include "SceneLoding.h"

#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CTransform.h"
#include"CLogoBack.h"
#include"CLogoTitle.h"

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
	CLogoBack* m_pLogo = CLogoBack::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Background", m_pLogo);
	CLogoTitle* m_pTitle = CLogoTitle::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Title", m_pTitle);

	return S_OK;
}

_int Logo::Update_Scene(const _float& fTimeDelta)
{
	

	if (CInputMgr::Get_Instance()->Key_Tap(DIK_U))
	{
		CRenderMgr::Get_Instance()->Get_PostProcessing()->Set_Assemble();
	}
	if (CInputMgr::Get_Instance()->Key_Tap(DIK_F1))
	{
		CScene* pScene = CSceneTestHS::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}

	else if(CInputMgr::Get_Instance()->Key_Tap(DIK_F2))
	{
		CScene* pScene = SceneStage1::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F3))
	{
		CScene* pScene = SceneStage2::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F4))
	{
		CScene* pScene = SceneStage3::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}
	else if (CInputMgr::Get_Instance()->Key_Tap(DIK_F5))
	{
		CScene* pScene = BossScene::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
	}

	CScene::Update_Scene(fTimeDelta);

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