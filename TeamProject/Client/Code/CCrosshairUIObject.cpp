#include "pch.h"
#include "Engine_Model.h"
#include "CCrosshairUIObject.h"

#include "CUiImage.h"
#include "CTransform.h"
#include "CModel.h"

#include "CResourceMgr.h"

CCrosshairUIObject::CCrosshairUIObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CCrosshairUIObject::~CCrosshairUIObject()
{
    Free();
}

HRESULT CCrosshairUIObject::Ready_GameObject()
{
    DefaultCubeModel tModel;
    
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CUiImage>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
    
    Get_Component<CTransform>()->Ready_Transform();

    //Ready_Resource 되어있는곳이 없어서 임시 ( 나중에 지워도 됨)
    CResourceMgr::Get_Instance()->Ready_Resource();

    CResourceMgr::Get_Instance()->Load_Texture(L"CROSS_DEFAULT.png");
    CResourceMgr::Get_Instance()->Load_Texture(L"CROSS_HOVER.png");

    Get_Component<CUiImage>()->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"CROSS_DEFAULT.png"));
    Get_Component<CUiImage>()->Set_Position(D3DXVECTOR2((WINCX * 0.5f)- 25, (WINCY * 0.5f)-25));
    Get_Component<CUiImage>()->Set_Scale(D3DXVECTOR2(1.f, 1.f));

    return S_OK;
}

_int CCrosshairUIObject::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);

    return 0;
}

void CCrosshairUIObject::Render_GameObject()
{

    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component();
    //Get_Component<CUiImage>()->Render_Component();
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
        pImg->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"CROSS_DEFAULT.png"));
        break;
    case CROSSHAIR_STATE::CROSS_HOVER:
        pImg->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"CROSS_HOVER.png"));
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
