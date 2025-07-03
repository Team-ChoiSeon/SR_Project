#include "pch.h"
#include "Engine_Define.h"
#include "CMainApp.h"

#include "CGraphicDev.h"

#include "CTimeMgr.h"
#include "CFrameMgr.h"
#include "CInputMgr.h"
#include "CSceneMgr.h"
#include "CLightMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CScene.h"
#include "Logo.h"

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

	if (FAILED(CInputMgr::Get_Instance()->Ready_InputDev(g_HInst, g_hWnd)))
		return E_FAIL;

	m_pDeviceClass->AddRef();

	m_pGraphicDev = m_pDeviceClass->Get_GraphicDev();
	m_pGraphicDev->AddRef();
  
	m_pScene = Logo::Create(m_pGraphicDev);
	CSceneMgr::Get_Instance()->Set_Scene(m_pScene);
	return S_OK;
}

int CMainApp::Update_MainApp(_float& fTimeDelta)
{
	CInputMgr::Get_Instance()->Update_InputDev();
	CSceneMgr::Get_Instance()->Update_Scene(fTimeDelta);

	return 0;
}

void CMainApp::LateUpdate_MainApp(_float& fTimeDelta)
{
	CInputMgr::Get_Instance()->LateUpdate_InputDev();
	CSceneMgr::Get_Instance()->LateUpdate_Scene(fTimeDelta);
}


void CMainApp::Render_MainApp()
{
	m_pDeviceClass->Render_Begin(D3DXCOLOR(0.f,0.f, 1.f, 1.f));
	CSceneMgr::Get_Instance()->Render_Scene();
  
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

	CTimeMgr::Get_Instance()->Destroy_Instance();
	CFrameMgr::Get_Instance()->Destroy_Instance();
	CInputMgr::Get_Instance()->Destroy_Instance();
	CSceneMgr::Get_Instance()->Destroy_Instance();
	CLightMgr::Get_Instance()->Destroy_Instance();
	CCollisionMgr::Get_Instance()->Destroy_Instance();
	CRenderMgr::Get_Instance()->Destroy_Instance();

	CGraphicDev::Get_Instance()->Destroy_Instance();
}
