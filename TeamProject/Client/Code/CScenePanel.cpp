#include "pch.h"
#include "CScenePanel.h"
#include "CTransform.h"
#include "CResourceMgr.h"
#include "CDialogue.h"
#include "CUiQuad.h"
#include "CResourceMgr.h"

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
    CDialogue* dialogue = CDialogue::Create(m_pGraphicDev);
    m_vecUI.push_back(dialogue);
    return S_OK;
}

_int CScenePanel::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    for (auto& ui : m_vecUI) {
        ui->Update_GameObject(fTimeDelta);
    }
    return 0;
}

void CScenePanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    for (auto& ui : m_vecUI) {
        ui->LateUpdate_GameObject(fTimeDelta);
    }
}

void CScenePanel::Free()
{
    for (auto& ui : m_vecUI) {
        Safe_Release(ui);
    }
    CGameObject::Free();
}
