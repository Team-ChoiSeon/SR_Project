#include "pch.h"
#include "Engine_Define.h"
#include "CLogoPanel.h"
#include "CTransform.h"
#include "CUiQuad.h"

CLogoPanel::CLogoPanel(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CLogoPanel::~CLogoPanel()
{
}

CLogoPanel* CLogoPanel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogoPanel* instance = new CLogoPanel(pGraphicDev);
    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }
    return instance;
}

HRESULT CLogoPanel::Ready_GameObject()
{
    m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
    m_pQuad->Set_Texture(L"UI/CinematicBar.png");
    m_pQuad->Set_Shader(L"g_UIShader.fx");
    m_pQuad->Set_Alpha(0.4);
    m_tPanel.Set_Size({ WINCX,WINCY });
    m_tPanel.Set_Pos({ WINCX/2,WINCY/2 });


    return S_OK;
}

_int CLogoPanel::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
    m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
    return 0;
}

void CLogoPanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLogoPanel::Free()
{
    CGameObject::Free();
}
