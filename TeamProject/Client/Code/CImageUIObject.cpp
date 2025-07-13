#include "pch.h"
#include "Engine_Model.h"
#include "CImageUIObject.h"

#include "CUiImage.h"
#include "CTransform.h"
#include "CTexture.h"

#include "CResourceMgr.h"


CImageUIObject::CImageUIObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CImageUIObject::~CImageUIObject()
{
    Free();
}

HRESULT CImageUIObject::Ready_GameObject()
{
    Add_Component<CUiImage>(ID_DYNAMIC, m_pGraphicDev);
    return S_OK;
}

_int CImageUIObject::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CImageUIObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CImageUIObject::Set_Texture(CTexture* pTex)
{
    Get_Component<CUiImage>()->Set_Texture(pTex);
}

void CImageUIObject::Set_Position(const D3DXVECTOR2& pos)
{
    Get_Component<CUiImage>()->Set_Position(pos);
}

void CImageUIObject::Set_RenderSize(_float width, _float height)
{
    Get_Component<CUiImage>()->Set_RenderSize(width, height);
}

CImageUIObject* CImageUIObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CImageUIObject* pImageUIObjec = new CImageUIObject(pGraphicDev);

    if (FAILED(pImageUIObjec->Ready_GameObject()))
    {
        Safe_Release(pImageUIObjec);
        MSG_BOX("CrosshairUI Create Failed");
        return nullptr;
    }

    return pImageUIObjec;
}


void CImageUIObject::Free()
{
    CGameObject::Free();
}
