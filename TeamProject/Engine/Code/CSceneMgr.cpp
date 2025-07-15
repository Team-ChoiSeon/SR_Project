#pragma once
#include "CSceneMgr.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr() : m_pCurScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
    Free();
}

HRESULT CSceneMgr::Ready_SceneManager(CScene* pScene)
{
    m_pCurScene = pScene;
    return S_OK;
}

HRESULT CSceneMgr::Set_Scene(CScene* pScene)
{

    if (nullptr == pScene) //null pointer
        return  E_FAIL;
    wstring name = pScene->Get_ScenePath();
    if (pScene->Get_ScenePath().empty()) //empty Path
        return E_FAIL;
 
    CScene* prevScene = m_pCurScene;
    m_pCurScene = m_pLoading;

    HRESULT hr = m_pLoading->LoadScene(prevScene,pScene);

    if (FAILED(hr)) {
        MessageBoxW(0, L"씬 로딩 경로 설정 재확인 필요", L"I told you so", MB_OK);
        m_pCurScene = prevScene;
        Safe_Release(pScene);
    }

    OutputDebugString("Set_Scene\n");
    return S_OK;
}

void CSceneMgr::Set_LoadingScene(CScene* pScene)
{
    m_pLoading = pScene;
}

HRESULT CSceneMgr::Set_Player(CGameObject* pPlayer)
{
    if (pPlayer == nullptr)
        return E_FAIL;

    m_pPlayer = pPlayer;
    return S_OK;
}

void CSceneMgr::Set_CurrentScene(CScene* pScene)
{
    m_pCurScene = pScene;
    m_pCurScene->Ready_Scene();
}

_int CSceneMgr::Update_Scene(const _float& fTimeDelta)
{
    
    if (nullptr == m_pCurScene)
        return -1;

  //  if (!m_pPlayer)
  //  {
  //      CLayer* pLayer = m_pCurScene->Get_Layer(LAYER_PLAYER);
  //      if (pLayer)
  //          m_pPlayer = pLayer->Get_GameObject(L"MyPlayer");
  //  }

    return m_pCurScene->Update_Scene(fTimeDelta);
}

void CSceneMgr::LateUpdate_Scene(const _float& fTimeDelta)
{
    m_pCurScene->LateUpdate_Scene(fTimeDelta);
}

void CSceneMgr::Free()
{       
    Safe_Release(m_pLoading);
    Safe_Release(m_pCurScene);
}