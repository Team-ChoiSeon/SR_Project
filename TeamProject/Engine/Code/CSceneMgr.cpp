#pragma once
#include "CSceneMgr.h"

IMPLEMENT_SINGLETON(CSceneMgr)

CSceneMgr::CSceneMgr() : m_pScene(nullptr)
{
}

CSceneMgr::~CSceneMgr()
{
    Free();
}


HRESULT CSceneMgr::Set_Scene(CScene* pScene)
{
    if (nullptr == pScene)
        return  E_FAIL;

 
    Safe_Release(m_pScene);

    m_pScene = pScene;

    OutputDebugString("Set_Scene\n");
    return S_OK;
}

HRESULT CSceneMgr::Set_Player(CGameObject* pPlayer)
{
    if (pPlayer == nullptr)
        return E_FAIL;

    m_pPlayer = pPlayer;
    return S_OK;
}

_int CSceneMgr::Update_Scene(const _float& fTimeDelta)
{
    if (nullptr == m_pScene)
        return -1;

    return m_pScene->Update_Scene(fTimeDelta);
}

void CSceneMgr::LateUpdate_Scene(const _float& fTimeDelta)
{
    m_pScene->LateUpdate_Scene(fTimeDelta);
}



void CSceneMgr::Free()
{       
    Safe_Release(m_pScene);
}