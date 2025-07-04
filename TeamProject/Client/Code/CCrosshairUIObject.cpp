#include "pch.h"
#include "CCrosshairUIObject.h"
#include "CUiImage.h"
#include "CResourceMgr.h"
#include "CTransform.h"

CCrosshairUIObject::CCrosshairUIObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
    CResourceMgr::Get_Instance()->Ready_Resource();
    CResourceMgr::Get_Instance()->Load_Texture(L"crosshair.png");
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CUiImage>(ID_DYNAMIC, m_pGraphicDev);
}

CCrosshairUIObject::~CCrosshairUIObject()
{
}

HRESULT CCrosshairUIObject::Ready_GameObject()
{
    Get_Component<CTransform>()->Ready_Transform();

    // CUiImage* pImg = Get_Component<CUiImage>();
    // if (!pImg) 
    //     return E_FAIL;

    //if (!)
    //    return E_FAIL;
    Get_Component<CUiImage>()->Ready_Image(CResourceMgr::Get_Instance()->Get_Texture(L"crosshair.png"));

    Get_Component<CUiImage>()->Set_Position(D3DXVECTOR2(WINCX/2, WINCY/2));
    Get_Component<CUiImage>()->Set_Scale(D3DXVECTOR2(40.f, 40.f));

    return S_OK;
}

_int CCrosshairUIObject::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

void CCrosshairUIObject::Render_GameObject()
{
    CUiImage* pImg = Get_Component<CUiImage>();
    if (pImg)
        pImg->Render_Component();
}

void CCrosshairUIObject::Set_State(CROSSHAIR_STATE eState)
{
    // 추후 변경
    if (m_eState == eState)
        return;

    m_eState = eState;
    Update_Texture_State();
}

void CCrosshairUIObject::Update_Texture_State()
{
    CUiImage* pImg = Get_Component<CUiImage>();
    if (!pImg)
        return;

    CTexture* pTex = nullptr;

    switch (m_eState)
    {
    case CROSSHAIR_STATE::CROSS_DEFAULT:
        break;
    case CROSSHAIR_STATE::CROSS_HOVER:
        break;
    case CROSSHAIR_STATE::CROSS_HOLD:
        break;
    }

    if (pTex)
        pImg->Set_Texture(pTex);
}