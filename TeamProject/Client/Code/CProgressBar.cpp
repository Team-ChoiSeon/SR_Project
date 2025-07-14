#include "pch.h"
#include "Engine_Model.h"
#include "CProgressBar.h"

#include "CTransform.h"

#include "CResourceMgr.h"
#include "CFactory.h"

#include "CImageUIObject.h"

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

    CResourceMgr::Get_Instance()->Ready_Resource();
    CResourceMgr::Get_Instance()->Load_Texture(L"UI/ProgressBar_Back.png");
    CResourceMgr::Get_Instance()->Load_Texture(L"UI/ProgressBar_Fill.png");

    m_pBackImage = CImageUIObject::Create(m_pGraphicDev);
    m_pFillImage = CImageUIObject::Create(m_pGraphicDev);

    m_pBackImage->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"UI/ProgressBar_Back.png"));
    m_pBackImage->Set_Position({ -245.f, 550.f });
    //m_pBackImage->Set_Position({ -420.f, 550.f });
    m_pBackImage->Set_RenderSize(150.f, 10.f);

    m_pFillImage->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"UI/ProgressBar_Fill.png"));
    m_pFillImage->Set_Position({ -245.f, 550.f });
    //m_pFillImage->Set_Position({ -420.f, 550.f });
    m_pFillImage->Set_RenderSize(150.f, 10.f);

    return S_OK;
}

_int CProgressBar::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    m_pBackImage->Update_GameObject(fTimeDelta);
    m_pFillImage->Update_GameObject(fTimeDelta);

    return 0;
}

void CProgressBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
    m_pBackImage->LateUpdate_GameObject(fTimeDelta);
    m_pFillImage->LateUpdate_GameObject(fTimeDelta);
}

void CProgressBar::Set_Progress(float fPercent)
{
    m_fProgress = max(0.f, min(fPercent, 1.f));
    float fWidth = -(150.f * m_fProgress);
    float fLeft = -245.f - 150.f;
    D3DXVECTOR2 vBaseScale = { 150.f, 10.f };
    m_pFillImage->Set_Position({ fLeft - fWidth, 550.f });
    m_pFillImage->Set_RenderSize(vBaseScale.x * m_fProgress, vBaseScale.y);
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
    Safe_Release(m_pBackImage);
    Safe_Release(m_pFillImage);

    CGameObject::Free();
}
