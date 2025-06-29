#include "pch.h"
#include "Engine_Define.h"
#include "CMainApp.h"

#include "CGraphicDev.h"

#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CInputMgr.h"
#include "Player.h"
#include "CTransform.h"
#include "TestFollowingCam.h"
#include "CCameraObject.h"

CMainApp::CMainApp()
	:m_pDeviceClass(nullptr)
	,m_pGraphicDev(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(CGraphicDev::Get_Instance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;

	if (FAILED(CInputMgr::Get_Instance()->Ready_InputDev(g_HInst, g_hWnd)))
		return E_FAIL;

	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	m_pPlayer = new Player(m_pGraphicDev);
	m_pPlayer->Ready_GameObject();

	m_pFFCam = TestFollowingCam::Create(m_pGraphicDev, m_pPlayer, {0.f, 3.f, -3.f});
	m_pFFCam->Ready_Fcam();

	if (FAILED(D3DXCreateFont(
		m_pGraphicDev,   
		20, 0,           
		FW_NORMAL,       
		1, FALSE,        
		DEFAULT_CHARSET, 
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"굴림",           // font name
		&m_pFont)))
	{
		return E_FAIL;
	}


	return S_OK;
}

int CMainApp::Update_MainApp(_float& fTimeDelta)
{
	CInputMgr::Get_Instance()->Update_InputDev();
	m_pPlayer->Update_GameObject(fTimeDelta);
	m_pFFCam->Update_FCam(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(_float& fTimeDelta)
{
	CInputMgr::Get_Instance()->LateUpdate_InputDev();
	m_pPlayer->LateUpdate_GameObject(fTimeDelta);
	m_pFFCam->LateUpdate_FCam();
}


void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f,0.f, 1.f, 1.f));

	m_pPlayer->Render_GameObject();
	m_pFFCam->Render_FCam();

	_vec3 v_playpos = m_pPlayer->GetPos();
	_vec3 v_camerapos = m_pFFCam->Get_Component<CTransform>(L"Transform")->Get_Pos();
	wchar_t buf[64];
	wchar_t buf2[64];
	wchar_t buf3[64];
	swprintf_s(buf, L"position : x: %.3f y: %.3f  z: %.3f", v_playpos.x, v_playpos.y, v_playpos.z);
	swprintf_s(buf2, L"cam position : x: %.3f y: %.3f  z: %.3f", v_camerapos.x, v_camerapos.y, v_camerapos.z);
	swprintf_s(buf3, L"");


	RECT rc = { 10, 10, 500, 30 };
	RECT rc2 = { 10, 30, 500, 50 };
	RECT rc3 = { 10, 50, 500, 70 };

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


	m_pDeviceClass->Render_End();
}

CMainApp* CMainApp::Create()
{
	CMainApp* pInstance = new CMainApp();
	if (FAILED(pInstance->Ready_MainApp())) {
		Engine::Safe_Release(pInstance);
		return nullptr;
	}
	return pInstance;
}

void CMainApp::Free()
{
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pPlayer);

	CTimeMgr::Get_Instance()->Destroy_Instance();
	CFrameMgr::Get_Instance()->Destroy_Instance();
	CInputMgr::Get_Instance()->Destroy_Instance();

	CGraphicDev::Get_Instance()->Destroy_Instance(); 
}
