#include "pch.h"
#include "CScenePanel.h"
#include "CTransform.h"
#include "CResourceMgr.h"
#include "CResourceMgr.h"
#include "CHealthPanel.h"
#include "CObjectInfo.h"

CScenePanel::CScenePanel(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CScenePanel::~CScenePanel()
{
}

CScenePanel* CScenePanel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CScenePanel* instance = new CScenePanel(pGraphicDev);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CScenePanel::Ready_GameObject()
{
    CHealthPanel* CHealthPanel = CHealthPanel::Create(m_pGraphicDev);
    CObjectInfo* CObjectInfo = CObjectInfo::Create(m_pGraphicDev);
    m_umUI.insert({ L"Health_UI",CHealthPanel});
    m_umUI.insert({ L"ObjectInfo_UI",CObjectInfo });
    return S_OK;
}

_int CScenePanel::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    for (auto& ui : m_umUI) {
            ui.second->Update_GameObject(fTimeDelta);
    }
    return 0;
}

void CScenePanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    for (auto& ui : m_umUI) {
        ui.second->LateUpdate_GameObject(fTimeDelta);
    }
}

void CScenePanel::Free()
{
    for (auto& ui : m_umUI) {
        Safe_Release(ui.second);
    }
    CGameObject::Free();
}
