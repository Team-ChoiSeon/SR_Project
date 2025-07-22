#include "pch.h"
#include "Engine_Define.h"
#include "CObjectInfo.h"
#include "CUiQuad.h"
#include "CTransform.h"
#include "CMainPlayer.h"
#include "CSceneMgr.h"

CObjectInfo::CObjectInfo(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CObjectInfo::~CObjectInfo()
{
}

CObjectInfo* CObjectInfo::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CObjectInfo* instance = new CObjectInfo(pGraphicDev);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CObjectInfo::Ready_GameObject()
{
    m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);

    m_tPanel.Set_Anchor(UIPanel::Anchor::RightCenter, {WINCX, WINCY});
    m_tPanel.Set_Size({60,60});
    m_pQuad->Set_Texture(L"UI/CinematicBar.png");
    m_pQuad->Set_Shader(L"g_UIShader.fx");
    m_pQuad->Set_Alpha(0.4);
    return S_OK;
}

_int CObjectInfo::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_pPlayer) {
        if (CGameObject* player = CSceneMgr::Get_Instance()->Get_Player()) {
            m_pPlayer = static_cast<CMainPlayer*>(player);
        }
        return 0 ;
    }

    if (CGameObject* picked = m_pPlayer->Get_PickObj()) {

    }

    return 0;
}

void CObjectInfo::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CObjectInfo::Free()
{
}
