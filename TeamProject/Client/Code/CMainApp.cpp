#include "pch.h"
#include "Engine_Define.h"
#include "CMainApp.h"

#include "CGraphicDev.h"

#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CInputMgr.h"
#include "Player.h"

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
	if (FAILED(CGraphicDev::GetInstance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;

	if (FAILED(CInputMgr::GetInstance()->Ready_InputDev(g_HInst, g_hWnd)))
		return E_FAIL;

	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	m_pPlayer = new Player(m_pGraphicDev);
	//m_pPlayer->AddRef();
	m_pPlayer->Ready_GameObject();

	if (FAILED(D3DXCreateFont(
		m_pGraphicDev,    // D3D device
		20, 0,             // 높이, 폭(0=비례)
		FW_NORMAL,         // 두께
		1, FALSE,          // MipLevels, Italic
		DEFAULT_CHARSET,   // CharSet
		OUT_DEFAULT_PRECIS,
		ANTIALIASED_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE,
		L"굴림",           // 폰트 이름
		&m_pFont)))
	{
		return E_FAIL;
	}


	return S_OK;
}

int CMainApp::Update_MainApp(_float& fTimeDelta)
{
	CInputMgr::GetInstance()->Update_InputDev();
	m_pPlayer->Update_GameObject(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(_float& fTimeDelta)
{
	CInputMgr::GetInstance()->LateUpdate_InputDev();
	m_pPlayer->LateUpdate_GameObject(fTimeDelta);
}


void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f, 0.f, 1.f, 1.f));

	m_pPlayer->Render_GameObject();

	_vec3 v_playpos = m_pPlayer->GetPos();
	wchar_t buf[64];
	swprintf_s(buf, L"position : %.3f    %.3f    %.3f", v_playpos.x, v_playpos.y, v_playpos.z);

	RECT rc = { 10, 10, 500, 30 };

	m_pFont->DrawTextW(
		nullptr,      // sprite (nullptr 가능)
		buf,          // 출력할 문자열
		-1,           // 문자열 길이(-1이면 널종료까지)
		&rc,
		DT_LEFT | DT_TOP,
		D3DCOLOR_ARGB(255, 255, 255, 255)
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


	CTimeMgr::GetInstance()->DestroyInstance();
	CFrameMgr::GetInstance()->DestroyInstance();
	CInputMgr::GetInstance()->DestroyInstance();

	CGraphicDev::GetInstance()->DestroyInstance(); 
}
