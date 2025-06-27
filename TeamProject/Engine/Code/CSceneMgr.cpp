#pragma once
#include "CSceneMgr.h"
#include "CRenderMgr.h"

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
    //CRenderMgr::GetInstance()->Clear_RenderGroup();
    m_pScene = pScene;

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

void CSceneMgr::Render_Scene(LPDIRECT3DDEVICE9 pGraphicDev)
{
    m_pScene->Render_Scene();
}

void CSceneMgr::Free()
{
    Safe_Release(m_pScene);
}