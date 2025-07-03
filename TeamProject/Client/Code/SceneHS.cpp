#pragma once
#include "pch.h"
#include "SceneHS.h"

#include "CTimeMgr.h"
#include "CInputMgr.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"

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
	//Init_Layers();
	Add_Layer(LAYER_OBJECT);
	Add_Layer(LAYER_CAMERA);

	m_pObjectLayer = Get_Layer(LAYER_OBJECT);;
	m_pCameraLayer = Get_Layer(LAYER_CAMERA);

	m_pDummy = DummyCube::Create(m_pGraphicDev);


	m_pLightObject = new CLightObject(m_pGraphicDev);
	if (FAILED(m_pLightObject->Ready_GameObject()))
		return E_FAIL;

	m_pTestLightMesh = new CTestLightMeshObject(m_pGraphicDev);
	if (FAILED(m_pTestLightMesh->Ready_GameObject()))
		return E_FAIL;

	m_pCrosshair = new CCrosshairUIObject(m_pGraphicDev);
	if (FAILED(m_pCrosshair->Ready_GameObject()))
		return E_FAIL;
	CUiMgr::Get_Instance()->AddUI(m_pCrosshair);

	//m_pFFCam = CFirstviewFollowingCamera::Create(m_pGraphicDev, m_pPlayer);
	m_pdummycam = CFirstviewFollowingCamera::Create(m_pGraphicDev, m_pDummy);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pdummycam);


	m_pObjectLayer->Add_GameObject(L"Dummy", (m_pDummy));
	m_pObjectLayer->Add_GameObject(L"TestLightMesh", (m_pTestLightMesh));
	m_pCameraLayer->Add_GameObject(L"dummycam", m_pdummycam);

	if (FAILED(D3DXCreateFont(
		m_pGraphicDev,
		20, 0,
		FW_NORMAL,
		1, FALSE,
		DEFAULT_CHARSET,
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"±¼¸²",           // font name
		&m_pFont)))
	{
		return E_FAIL;
	}

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int SceneHS::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	m_pLightObject->Update_GameObject(fTimeDelta);
	m_pTestLightMesh->Update_GameObject(fTimeDelta);

	if (m_pCrosshair)
		m_pCrosshair->Update_GameObject(fTimeDelta);

	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);

	return 0;
}

void SceneHS::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);

	m_pLightObject->LateUpdate_GameObject(fTimeDelta);
	m_pTestLightMesh->LateUpdate_GameObject(fTimeDelta);
	
	//Engine::CLightMgr::Get_Instance()->UpdateLights({ 0.f, 0.f, -10.f });
	Engine::CLightMgr::Get_Instance()->UpdateLights(CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>()->Get_Pos());
	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
}

void SceneHS::Render_Scene()
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Render_Layer();

	m_pLightObject->Render_GameObject();
	m_pTestLightMesh->Render_GameObject();


	CUiMgr::Get_Instance()->RenderUI();

	//if (m_pCrosshair)
	//	m_pCrosshair->Render_GameObject();

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

	Safe_Release(m_pFont);
	Safe_Release(m_pDummy);
	Safe_Release(m_pLightObject);
	Safe_Release(m_pTestLightMesh);
	Safe_Release(m_pdummycam);
	Safe_Release(m_pCrosshair);
	
	CCameraMgr::Destroy_Instance();
	CUiMgr::Destroy_Instance();

	CScene::Free();
}