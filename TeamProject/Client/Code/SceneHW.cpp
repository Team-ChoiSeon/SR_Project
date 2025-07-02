#pragma once
#include "pch.h"
#include "SceneHW.h"
#include "Player.h"
#include "CFirstviewFollowingCamera.h"
#include "DummyCube.h""
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CInputMgr.h"

SceneHW::SceneHW(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev), m_pGraphicDev(pGraphicDev)
{

}

SceneHW::~SceneHW()
{
}

HRESULT SceneHW::Ready_Scene()
{
	Add_Layer(LAYER_PLAYER);
	Add_Layer(LAYER_OBJECT);
	Add_Layer(LAYER_CAMERA);
	m_pPlayerLayer = Get_Layer(LAYER_PLAYER);
	m_pObjectLayer = Get_Layer(LAYER_OBJECT);
	m_pCameraLayer = Get_Layer(LAYER_CAMERA);
	m_pPlayer = Player::Create(m_pGraphicDev);
	m_pDummy = DummyCube::Create(m_pGraphicDev);
	m_pFFCam = CFirstviewFollowingCamera::Create(m_pGraphicDev, m_pPlayer);
	m_pdummycam = CFirstviewFollowingCamera::Create(m_pGraphicDev, m_pDummy);
	m_pPlayerLayer->Add_GameObject(L"Player", (m_pPlayer));
	m_pObjectLayer->Add_GameObject(L"Dummy", (m_pDummy));
	m_pCameraLayer->Add_GameObject(L"FFCam", (m_pFFCam));
	m_pCameraLayer->Add_GameObject(L"dummycam", m_pdummycam);

	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);

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
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);


	if (CInputMgr::Get_Instance()->Key_Away(DIK_C))
	{
		if (m_bCamPlayer)
			m_bCamPlayer = false;
		else
			m_bCamPlayer = true;
	}

	if (m_bCamPlayer)
		CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);
	else
		CCameraMgr::Get_Instance()->Set_MainCamera(m_pdummycam);


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

	_vec3 v_playpos = m_pPlayer->GetPos();
	_vec3 v_campos = CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>()->Get_Pos();
	//_vec3 dummylook = m_pDummy->Get_Component<CTransform>()->Get_Info(INFO_LOOK);
	//_vec3 camlook = m_pdummycam->Get_Component<CTransform>()->Get_Info(INFO_LOOK);
	wchar_t buf[64];
	wchar_t buf2[64];
	wchar_t buf3[64];
	wchar_t buf4[64];
	swprintf_s(buf, L"position : x: %.3f y: %.3f  z: %.3f", v_playpos.x, v_playpos.y, v_playpos.z);
	swprintf_s(buf2, L"cam position: x: %.3f y: %.3f z: %.3f ", v_campos.x, v_campos.y, v_campos.z);
	swprintf_s(buf3, L"");
	swprintf_s(buf4, L"");

	RECT rc = { 10, 10, 500, 30 };
	RECT rc2 = { 10, 30, 500, 50 };
	RECT rc3 = { 10, 50, 500, 70 };
	RECT rc4 = { 10, 70, 500, 90 };

	m_pFont->DrawTextW(
		nullptr,
		buf,
		-1,
		&rc,
		DT_LEFT | DT_TOP,
		D3DCOLOR_ARGB(255, 255, 0, 0)
	);

	m_pFont->DrawTextW(
		nullptr,
		buf2,
		-1,
		&rc2,
		DT_LEFT | DT_TOP,
		D3DCOLOR_ARGB(255, 255, 0, 0)
	);
	m_pFont->DrawTextW(
		nullptr,
		buf3,
		-1,
		&rc3,
		DT_LEFT | DT_TOP,
		D3DCOLOR_ARGB(255, 255, 0, 0)
	);
	m_pFont->DrawTextW(
		nullptr,
		buf4,
		-1,
		&rc4,
		DT_LEFT | DT_TOP,
		D3DCOLOR_ARGB(255, 255, 0, 0)
	);
}

void SceneHW::Free()
{
	Safe_Release(m_pCameraLayer);
	Safe_Release(m_pPlayerLayer);
	Safe_Release(m_pObjectLayer);
	Safe_Release(m_pPlayer);
	Safe_Release(m_pDummy);
	Safe_Release(m_pFFCam);
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphicDev);
	
	CCameraMgr::Destroy_Instance();

}

