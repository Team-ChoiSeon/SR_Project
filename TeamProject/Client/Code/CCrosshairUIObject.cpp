#include "pch.h"
#include "CCrosshairUIObject.h"
#include "CUiImage.h"
#include "CResourceMgr.h"
#include "CTransform.h"

CCrosshairUIObject::CCrosshairUIObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CUiImage>(ID_DYNAMIC, m_pGraphicDev);
}

CCrosshairUIObject::~CCrosshairUIObject()
{
    Free();
}

HRESULT CCrosshairUIObject::Ready_GameObject()
{
    Get_Component<CTransform>()->Ready_Transform();

    //Ready_Resource 되어있는곳이 없어서 임시 ( 나중에 지워도 됨)
    CResourceMgr::Get_Instance()->Ready_Resource();

    CResourceMgr::Get_Instance()->Load_Texture(L"CROSS_DEFAULT.png");
    CResourceMgr::Get_Instance()->Load_Texture(L"CROSS_HOVER.png");

    //CTexture* pt = CResourceMgr::Get_Instance()->Get_Texture(L"CROSS_DEFAULT.png");
    Get_Component<CUiImage>()->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"CROSS_DEFAULT.png"));
    Get_Component<CUiImage>()->Set_Position(D3DXVECTOR2((WINCX * 0.5f)- 25, (WINCY * 0.5f)-25));
    Get_Component<CUiImage>()->Set_Scale(D3DXVECTOR2(1.f, 1.f));

    return S_OK;
}

_int CCrosshairUIObject::Update_GameObject(const _float& fTimeDelta)
{
    return 0;
}

void CCrosshairUIObject::Render_GameObject()
{
    Get_Component<CUiImage>()->Render_Component();
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

    switch (m_eState)
    {
    case CROSSHAIR_STATE::CROSS_DEFAULT:
        //이곳에서 상태가 바뀐다.
        // pImg->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"crosshair_default.png"));
        //pTex = 
        break;
    case CROSSHAIR_STATE::CROSS_HOVER:
        break;
    case CROSSHAIR_STATE::CROSS_HOLD:
        break;
    }

}

CCrosshairUIObject* CCrosshairUIObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CCrosshairUIObject* pCrosshairUIObject = new CCrosshairUIObject(pGraphicDev);

    if (FAILED(pCrosshairUIObject->Ready_GameObject()))
    {
        Safe_Release(pCrosshairUIObject);
        MSG_BOX("CrosshairUI Create Failed");
        return nullptr;
    }

    return pCrosshairUIObject;
}


void CCrosshairUIObject::Free()
{
    CGameObject::Free();
}
