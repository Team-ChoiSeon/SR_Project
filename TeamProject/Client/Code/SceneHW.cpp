#pragma once
#include "pch.h"
#include "SceneHW.h"
#include "Player.h"
#include "CFirstviewFollowingCamera.h"
#include "DummyCube.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CInputMgr.h"
#include "CTransform.h"
#include "CPickingMgr.h"
#include "CFloatingCube.h"
#include "CDirectionalCube.h"
#include "CImpulseCube.h"

SceneHW::SceneHW(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CScene(pGraphicDev)
{
}

SceneHW::~SceneHW()
{
}

HRESULT SceneHW::Ready_Scene()
{
	Init_Layers();
	m_pPlayer = Player::Create(m_pGraphicDev);
	m_pDummy = DummyCube::Create(m_pGraphicDev);
	m_pFFCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	m_pFFCam->Set_Target(m_pPlayer);
	m_pdummycam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	m_pdummycam->Set_Target(m_pDummy);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwFloatingCube", CFloatingCube::Create(m_pGraphicDev));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwDirectionalCube", CDirectionalCube::Create(m_pGraphicDev));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwImpulseCube", CImpulseCube::Create(m_pGraphicDev));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwOnewayCube", CDirectionalCube::Create(m_pGraphicDev));

	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"hwPlayer", m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwdummy", m_pDummy);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"hwffcam", m_pFFCam);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"hwdummycam", m_pdummycam);

	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwFloatingCube"))->Set_Info({ -20.f, -20.f, 20.f }, { 1.f, 1.f, 0.f }, 50.f ,20.f, 1.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwDirectionalCube"))->Set_Info({ 0.f, 10.f, 10.f }, { 1.f, 0.f, 0.f }, -10.f, 10.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwDirectionalCube"))->Set_Grab(true);
	dynamic_cast<CImpulseCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwImpulseCube"))->Set_Info({ 10.f, 0.f, 10.f });
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwOnewayCube"))->Set_Info({ 0.f, -10.f, 10.f }, { 1.f, 0.f, 0.f }, 10.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwOnewayCube"))->Set_Grab(true);

	CPickingMgr::Get_Instance()->Ready_Picking(m_pGraphicDev, g_hWnd);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

SceneHW* SceneHW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneHW* pScene = new SceneHW(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

int SceneHW::Update_Scene(const _float& fTimeDelta)
{
	CPickingMgr::Get_Instance()->Update_Picking(fTimeDelta);
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);


	if (CInputMgr::Get_Instance()->Key_Away(DIK_C))
		m_bCamPlayer = !m_bCamPlayer;

	float dx = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMS_X) / 20.f;
	float dy = CInputMgr::Get_Instance()->Get_DIMouseMove(MOUSEMOVESTATE::DIMD_Y) / 20.f;
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwDirectionalCube"))->Set_CursorVec({ dx, 0.f, dy });
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwOnewayCube"))->Set_CursorVec({ dx, 0.f, dy });

	if (m_bCamPlayer)
		CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);
	else
		CCameraMgr::Get_Instance()->Set_MainCamera(m_pdummycam);

	if (CPickingMgr::Get_Instance()->Get_HitTarget() == m_pDummy)
	{
		OutputDebugStringW(L"[Debug] Hit!	\n");
	}


	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);

	return 0;
}

void SceneHW::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
}

void SceneHW::Render_Scene()
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Render_Layer();
}

void SceneHW::Free()
{
	CScene::Free();

	CCameraMgr::Destroy_Instance();
	CPickingMgr::Destroy_Instance();
}

