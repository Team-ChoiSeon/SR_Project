#include "pch.h"
#include "Engine_Define.h"
#include "CMainApp.h"

#include "CGraphicDev.h"

#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CInputMgr.h"
#include "CSoundMgr.h"

#include "CSceneMgr.h"
#include "SceneLoding.h"
#include "CScene.h"
#include "Logo.h"

#include "CRenderMgr.h"
#include "CShaderMgr.h"
#include "CResourceMgr.h"

#include "CUIMgr.h"
#include "CPickingMgr.h"

#include "CCollisionMgr.h"
#include "CCameraMgr.h"
#include "CLightMgr.h"

#include "CGuiSystem.h"


CMainApp::CMainApp()
	:m_pDeviceClass(nullptr)
	, m_pGraphicDev(nullptr)
{
}

CMainApp::~CMainApp()
{
}

HRESULT CMainApp::Ready_MainApp()
{
	if (FAILED(CGraphicDev::Get_Instance()->Ready_GraphicDev(g_hWnd, MODE_WIN, WINCX, WINCY, &m_pDeviceClass)))
		return E_FAIL;

	m_pDeviceClass->AddRef();
	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();

	CInputMgr::Get_Instance()->Ready_InputDev(g_HInst, g_hWnd);
	CResourceMgr::Get_Instance()->Ready_Resource();
	CShaderMgr::Get_Instance()->Ready_Shader(m_pGraphicDev);
	CRenderMgr::Get_Instance()->Ready_RenderMgr();

	CUiMgr::Get_Instance()->Ready_UiMgr();
	CPickingMgr::Get_Instance()->Ready_Picking(m_pGraphicDev, g_hWnd);

	// CameraMgr, LightMgr, CollisionMgr 초기화는 필요시 추가
	CSoundMgr::Get_Instance()->Ready_Sound();

	//CSoundMgr::Get_Instance()->Play("test", "BGM");

	//First Scene Setting
	m_pScene = Logo::Create(m_pGraphicDev);
	CSceneMgr::Get_Instance()->Ready_SceneManager(m_pScene);
	SceneLoding* loadingScene = SceneLoding::Create(m_pGraphicDev);
	//Loading Scene Setting
	CSceneMgr::Get_Instance()->Set_LoadingScene(loadingScene);
	CGuiSystem::Get_Instance()->Ready_GUI(g_hWnd);
	return S_OK;
}

int CMainApp::Update_MainApp(_float& fTimeDelta)
{
	CInputMgr::Get_Instance()->Update_InputDev();
	CPickingMgr::Get_Instance()->Update_Picking(fTimeDelta);

	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);
	CCollisionMgr::Get_Instance()->Update_Collision();

	CSceneMgr::Get_Instance()->Update_Scene(fTimeDelta);

	CSoundMgr::Get_Instance()->Update_Sound();

	return 0;
}

void CMainApp::LateUpdate_MainApp(_float& fTimeDelta)
{
	CInputMgr::Get_Instance()->LateUpdate_InputDev();
	CPickingMgr::Get_Instance()->LateUpdate_Picking(fTimeDelta);

	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
	CCollisionMgr::Get_Instance()->LateUpdate_Collision();

	CSceneMgr::Get_Instance()->LateUpdate_Scene(fTimeDelta);
}


void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f,0.f, 1.f, 1.f));
	CRenderMgr::Get_Instance()->Render(m_pGraphicDev);
	CGuiSystem::Get_Instance()->Render_GUI();
	
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
	// 1. 사용자 Scene 먼저 정리 (→ GameObject, Component까지)
	CSceneMgr::Get_Instance()->Destroy_Instance();

	// 2. 이들보다 아래 계층의 매니저 해제
	CCameraMgr::Get_Instance()->Destroy_Instance();
	CCollisionMgr::Get_Instance()->Destroy_Instance();
	CRenderMgr::Get_Instance()->Destroy_Instance();

	// 3. 그 외 매니저들
	CInputMgr::Get_Instance()->Destroy_Instance();
	CSoundMgr::Get_Instance()->Destroy_Instance();
	CFrameMgr::Get_Instance()->Destroy_Instance();
	CTimeMgr::Get_Instance()->Destroy_Instance();

	CUiMgr::Destroy_Instance();
	CLightMgr::Get_Instance()->Destroy_Instance();
	CPickingMgr::Get_Instance()->Destroy_Instance();
	
	// 4. 리소스, 디바이스
	CResourceMgr::Get_Instance()->Destroy_Instance();
	CShaderMgr::Get_Instance()->Destroy_Instance();
	CGraphicDev::Get_Instance()->Destroy_Instance();

	//5. GUI 시스템(디버그)
	CGuiSystem::Get_Instance()->Destroy_Instance();

	Safe_Release(m_pDeviceClass);
	Safe_Release(m_pDeviceClass);
}
