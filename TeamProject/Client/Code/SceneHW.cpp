#pragma once
#include "pch.h"
#include "SceneHW.h"
#include "Player.h"
#include "CFirstviewFollowingCamera.h"
#include "DummyCube.h""
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CFrameMgr.h"

SceneHW::SceneHW(LPDIRECT3DDEVICE9 pGraphicDev) : CScene(pGraphicDev)
{
	m_pGraphicDev = pGraphicDev;
}

SceneHW::~SceneHW()
{
}

HRESULT SceneHW::Ready_Scene()
{
	m_pPlayer = new Player(m_pGraphicDev);
	m_pPlayer->Ready_GameObject();

	m_pDummy = DummyCube::Create_Dummy(m_pGraphicDev);
	m_pDummy->Ready_Dummy();

	m_pFFcam = CFirstviewFollowingCamera::Create(m_pGraphicDev, m_pPlayer);


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

	m_pPlayer->Update_GameObject(fTimeDelta);
	m_pDummy->Update_Dummy(fTimeDelta);
	m_pFFcam->Update_FFCam(fTimeDelta);

	return 0;
}

void SceneHW::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
	m_pPlayer->LateUpdate_GameObject(fTimeDelta);
	m_pDummy->LateUpdate_Dummy(fTimeDelta);
	m_pFFcam->LateUpdate_FFCam();
}

void SceneHW::Render_Scene()
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Render_Layer();

	m_pPlayer->Render_GameObject();
	m_pDummy->Render_GameObject();
	m_pFFcam->Render_FFCam();

	_vec3 v_playpos = m_pPlayer->GetPos();
	_vec3 v_campos = m_pFFcam->Get_Component<CTransform>()->Get_Pos();
	_vec3 camlook = m_pFFcam->Get_Component<CCamera>()->Get_Look();
	_vec3 camrot = m_pFFcam->Get_Component<CTransform>()->Get_Angle();
	wchar_t buf[64];
	wchar_t buf2[64];
	wchar_t buf3[64];
	wchar_t buf4[64];
	swprintf_s(buf, L"position : x: %.3f y: %.3f  z: %.3f", v_playpos.x, v_playpos.y, v_playpos.z);
	swprintf_s(buf2, L"cam position: x: %.3f y: %.3f z: %.3f ", v_campos.x, v_campos.y, v_campos.z);
	swprintf_s(buf3, L"cam look: x: %.3f y: %.3f z: %.3f", camlook.x, camlook.y, camlook.z);
	swprintf_s(buf4, L"cam rot: x: %.3f y: %.3f z: %.3f", camrot.x, camrot.y, camrot.z);

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
	Safe_Release(m_pPlayer);
	Safe_Release(m_pDummy);
	Safe_Release(m_pFFcam);
	Safe_Release(m_pFont);
	Safe_Release(m_pGraphicDev);

}

