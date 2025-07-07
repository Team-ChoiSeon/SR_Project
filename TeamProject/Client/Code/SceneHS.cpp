#pragma once
#include "pch.h"
#include "SceneHS.h"

#include "CTimeMgr.h"
#include "CInputMgr.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CResourceMgr.h"
#include "CPickingMgr.h"

#include "CPlayer.h"
#include "Player.h"
#include "CLightObject.h"
#include "CTestLightMeshObject.h"
#include "CCrosshairUIObject.h"
#include "DummyCube.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"


SceneHS::SceneHS(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pGraphicDev(pGraphicDev)
{

}

SceneHS::~SceneHS()
{
}

HRESULT SceneHS::Ready_Scene()
{
	CUiMgr::Get_Instance()->Ready_UiMgr();

	Init_Layers();

	m_pPlayer = Player::Create(m_pGraphicDev);
	m_pLightObject = CLightObject::Create(m_pGraphicDev);
	m_pTestLightMesh = CTestLightMeshObject::Create(m_pGraphicDev);
	m_pCrosshair = CCrosshairUIObject::Create(m_pGraphicDev);
	m_pDummy = DummyCube::Create(m_pGraphicDev);

	CFirstviewFollowingCamera* m_pFFCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	m_pFFCam->Set_Target(m_pPlayer);

	m_pdummycam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	m_pdummycam->Set_Target(m_pDummy);

	//m_pObjectLayer = Get_Layer(LAYER_OBJECT);
	//m_pCameraLayer = Get_Layer(LAYER_CAMERA);

	CUiMgr::Get_Instance()->AddUI(m_pCrosshair);

	//m_pObjectLayer->Add_GameObject(L"Dummy", (m_pDummy));
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", m_pPlayer);

	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"Dummy", (m_pDummy));
	Get_Layer(LAYER_LIGHT)->Add_GameObject(L"TestLightMesh", (m_pTestLightMesh));
	Get_Layer(LAYER_LIGHT)->Add_GameObject(L"LightObject", (m_pLightObject));
	
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", (m_pCrosshair));

	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"ffcam", m_pFFCam);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"dummycam", m_pdummycam);

	CPickingMgr::Get_Instance()->Ready_Picking(m_pGraphicDev, g_hWnd);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int SceneHS::Update_Scene(const _float& fTimeDelta)
{
	CPickingMgr::Get_Instance()->Update_Picking(fTimeDelta);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);
	if (CPickingMgr::Get_Instance()->Get_HitTarget() == m_pDummy)
	{
		// 피킹 처리시 일어나는 부분
		m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOVER);
		OutputDebugStringW(L"[Debug] Hit!	\n");
	}
	else {
		m_pCrosshair->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_DEFAULT);
	}

	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);

	// m_pLightObject->Update_GameObject(fTimeDelta);
	// m_pTestLightMesh->Update_GameObject(fTimeDelta);
	// 
	// if (m_pCrosshair)
	// 	m_pCrosshair->Update_GameObject(fTimeDelta);

	// CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);

	return 0;
}

void SceneHS::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);

	CLightMgr::Get_Instance()->UpdateLights(CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>()->Get_Pos());
	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);

	// m_pLightObject->LateUpdate_GameObject(fTimeDelta);
	// m_pTestLightMesh->LateUpdate_GameObject(fTimeDelta);
	
	//Engine::CLightMgr::Get_Instance()->UpdateLights({ 0.f, 0.f, -10.f });
	// CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
}

void SceneHS::Render_Scene()
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Render_Layer();

	m_pLightObject->Render_GameObject();
	m_pTestLightMesh->Render_GameObject();


	CUiMgr::Get_Instance()->RenderUI();

	wchar_t buf[64];
	swprintf_s(buf, L"test : HS");

	RECT rc = { 10, 10, 500, 30 };
	m_pFont->DrawTextW(
		nullptr,
		buf,
		-1,
		&rc,
		DT_LEFT | DT_TOP,
		D3DCOLOR_ARGB(255, 255, 0, 0)
	);
}

SceneHS* SceneHS::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneHS* pScene = new SceneHS(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneHS::Free()
{
	Safe_Release(m_pObjectLayer);
	Safe_Release(m_pCameraLayer);
	
	Safe_Release(m_pPlayer);
	Safe_Release(m_pFont);
	Safe_Release(m_pDummy);
	Safe_Release(m_pLightObject);
	Safe_Release(m_pTestLightMesh);
	Safe_Release(m_pdummycam);
	Safe_Release(m_pCrosshair);
	
	

	Remove_Layer(LAYER_OBJECT);
	Remove_Layer(LAYER_CAMERA);

	CCameraMgr::Destroy_Instance();
	CPickingMgr::Destroy_Instance();
	CUiMgr::Destroy_Instance();
	CResourceMgr::Destroy_Instance();

	CScene::Free();
}