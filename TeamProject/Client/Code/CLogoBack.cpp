#include "pch.h"
#include "CLogoBack.h"
#include "Engine_Define.h"
#include "CUiQuad.h"

CLogoBack::CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject((pGraphicDev))
{

}

CLogoBack::~CLogoBack()
{
}

CLogoBack* CLogoBack::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLogoBack* instance = new CLogoBack(pGraphicDev);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CLogoBack::Ready_GameObject()
{
    m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
    m_pQuad->Set_Texture(L"UI/BackGround.png");
    m_pQuad->Set_Shader(L"g_UIShader.fx");
    m_pQuad->Set_Alpha(1.f);
    m_pQuad->Set_Post(true);
    m_tPanel.Set_Size({ WINCX,WINCY });
    m_tPanel.Set_Pos({ WINCX*0.5f,WINCY * 0.5f });
    
    return S_OK;
}

_int CLogoBack::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
    m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
    return 0;
}

void CLogoBack::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLogoBack::Free()
{
    CGameObject::Free();
}
