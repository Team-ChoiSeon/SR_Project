#include "pch.h"
#include "CInfoSlot.h"
#include "CUiQuad.h"
#include "CTransform.h"

CInfoSlot::CInfoSlot(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CInfoSlot::~CInfoSlot()
{
}

CInfoSlot* CInfoSlot::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CInfoSlot* instance = new CInfoSlot(pGraphicDev);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CInfoSlot::Ready_GameObject()
{
    m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);

    m_tPanel.Set_Size({ 20,20 });

    m_pQuad->Set_Texture(L"UI/Codex_Icon.png");
    m_pQuad->Set_Shader(L"g_UIShader.fx");
    //m_pQuad->Set_Alpha(0.4);

    return S_OK;
}

_int CInfoSlot::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
    m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });

    return 0;
}

void CInfoSlot::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CInfoSlot::Set_Pos(_vec2 pos)
{
    m_tPanel.Set_Pos(pos);
}

void CInfoSlot::Free()
{
    CGameObject::Free();
}
