#include "pch.h"
#include "Engine_Model.h"
#include "CProgressBar.h"

#include "CUiImage.h"
#include "CTransform.h"
#include "CTexture.h"

#include "CResourceMgr.h"
#include "CFactory.h"

CProgressBar::CProgressBar(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CProgressBar::~CProgressBar()
{
    Free();
}
HRESULT CProgressBar::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CUiImage>(ID_DYNAMIC, m_pGraphicDev);

    Get_Component<CTransform>()->Ready_Transform();

    CResourceMgr::Get_Instance()->Ready_Resource();
    CResourceMgr::Get_Instance()->Load_Texture(L"UI/ProgressBar_Fill.png");

    CUiImage* pImage = Get_Component<CUiImage>();
    pImage->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"UI/ProgressBar_Fill.png"));
    pImage->Set_Position({ 100.f, 100.f }); // 기본 위치
    pImage->Set_Scale({ 200.f, 20.f });     // 기본 사이즈

    return S_OK;
}

_int CProgressBar::Update_GameObject(const _float& fTimeDelta)
{
    CUiImage* pImage = Get_Component<CUiImage>();
    D3DXVECTOR2 vBaseScale = { 200.f, 20.f }; // 원래 스케일
    pImage->Set_Scale({ vBaseScale.x * m_fProgress, vBaseScale.y });

    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);

    return 0;
}

void CProgressBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component();

}

void CProgressBar::Render_GameObject()
{
}

void CProgressBar::Set_Progress(float fPercent)
{
    m_fProgress = max(0.f, min(fPercent, 1.f));
}

CProgressBar* CProgressBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CProgressBar* pProgressBar = new CProgressBar(pGraphicDev);

    if (FAILED(pProgressBar->Ready_GameObject()))
    {
        Safe_Release(pProgressBar);
        MSG_BOX("CrosshairUI Create Failed");
        return nullptr;
    }

    return pProgressBar;
}


void CProgressBar::Free()
{
    CGameObject::Free();
}

//REGISTER_GAMEOBJECT(CProgressBar)