#include "pch.h"
#include "CScenePanel.h"
#include "CTransform.h"
#include "CResourceMgr.h"
#include "CResourceMgr.h"
#include "CHealthPanel.h"
#include "CObjectInfo.h"
#include "CStageHint.h"

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
    CStageHint* CStageHint = CStageHint::Create(m_pGraphicDev);
    m_umUI.insert({ L"Health_UI",CHealthPanel});
    m_umUI.insert({ L"ObjectInfo_UI",CObjectInfo });
    m_umUI.insert({ L"Stage_Hint",CStageHint });
    return S_OK;
}

_int CScenePanel::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    m_umUI[L"Health_UI"]->Update_GameObject(fTimeDelta);
    m_umUI[L"Stage_Hint"]->Update_GameObject(fTimeDelta);

    if(m_bInfo)
        m_umUI[L"ObjectInfo_UI"]->Update_GameObject(fTimeDelta);
    return 0;
}

void CScenePanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    m_umUI[L"Health_UI"]->LateUpdate_GameObject(fTimeDelta);
    m_umUI[L"Stage_Hint"]->LateUpdate_GameObject(fTimeDelta);
    if (m_bInfo)
        m_umUI[L"ObjectInfo_UI"]->LateUpdate_GameObject(fTimeDelta);
}

void CScenePanel::Set_StageHint(const wstring& Hint)
{
    static_cast<CStageHint*>(m_umUI[L"Stage_Hint"])->Add_Hint(Hint);
}

void CScenePanel::Free()
{
    for (auto& ui : m_umUI) {
        Safe_Release(ui.second);
    }
    CGameObject::Free();
}
