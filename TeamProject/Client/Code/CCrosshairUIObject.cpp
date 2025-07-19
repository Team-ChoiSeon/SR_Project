#include "pch.h"
#include "Engine_Model.h"
#include "CCrosshairUIObject.h"

#include "CUiImage.h"
#include "CTransform.h"
#include "CModel.h"
#include "CTexture.h"

#include "CResourceMgr.h"
#include "CFactory.h"
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
    
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CUiImage>(ID_DYNAMIC, m_pGraphicDev);
    
    Get_Component<CTransform>()->Ready_Component();

    //Ready_Resource 되어있는곳이 없어서 임시 ( 나중에 지워도 됨)
    CResourceMgr::Get_Instance()->Ready_Resource();

    CResourceMgr::Get_Instance()->Load_Texture(L"Crosshair/Default.png");
    CResourceMgr::Get_Instance()->Load_Texture(L"Crosshair/Hover.png");
    CResourceMgr::Get_Instance()->Load_Texture(L"Crosshair/Hold.png");

    m_vecHoverAnim.push_back(CResourceMgr::Get_Instance()->Get_Texture(L"Crosshair/Default.png"));
    for (int i = 1; i <= 9; ++i)
    {
        wchar_t buf[64];
        swprintf_s(buf, L"Crosshair/Hovering%d.png", i);
        CResourceMgr::Get_Instance()->Load_Texture(buf);
        m_vecHoverAnim.push_back(CResourceMgr::Get_Instance()->Get_Texture(buf));
    }

    m_vecHoldAnim.push_back(CResourceMgr::Get_Instance()->Get_Texture(L"Crosshair/Hover.png"));
    for (int i = 1; i <= 8; ++i)
    {
        wchar_t buf[64];
        swprintf_s(buf, L"Crosshair/Holding%d.png", i);
        CResourceMgr::Get_Instance()->Load_Texture(buf);
        m_vecHoldAnim.push_back(CResourceMgr::Get_Instance()->Get_Texture(buf));
    }

    m_eState = CROSSHAIR_STATE::CROSS_DEFAULT;

    Get_Component<CUiImage>()->Set_Texture(CResourceMgr::Get_Instance()->Get_Texture(L"Crosshair/Default.png"));
    Get_Component<CUiImage>()->Set_Position(D3DXVECTOR2((WINCX * 0.5f)- 60, (WINCY * 0.5f)-60));
    //Get_Component<CUiImage>()->Set_RenderSize(120.f, 120.f);
    Get_Component<CUiImage>()->Set_Scale(D3DXVECTOR2(1.f, 1.f));

    CFactory::Save_Prefab(this, "CCrosshairUIObject");

    return S_OK;
}

_int CCrosshairUIObject::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    Update_Animation(fTimeDelta);

    return 0;
}

void CCrosshairUIObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CCrosshairUIObject::Set_State(CROSSHAIR_STATE eState)
{
    if (m_eState == eState || m_bAnimating)
        return;

    if (m_eState == CROSSHAIR_STATE::CROSS_DEFAULT && eState == CROSSHAIR_STATE::CROSS_HOLD)
        m_eTransitionState = CROSSHAIR_TRANSITION::DEFAULT_TO_HOVER;
    else if (m_eState == CROSSHAIR_STATE::CROSS_HOLD && eState == CROSSHAIR_STATE::CROSS_DEFAULT)
        m_eTransitionState = CROSSHAIR_TRANSITION::HOLD_TO_HOVER;
    else
    {
        m_ePrevState = m_eState;
        m_eState = eState;
    }
    m_iAnimFrame = 0;
    m_fAnimTime = 0.f;
    m_bAnimating = true;
}

void CCrosshairUIObject::Update_Animation(const _float& fTimeDelta)
{
    if (!m_bAnimating)
        return;

    m_fAnimTime += fTimeDelta;
    const float frameDuration = 0.02f;

    if (m_fAnimTime < frameDuration)
        return;

    m_fAnimTime = 0.f;

    const std::vector<CTexture*>* pFrames = nullptr;
    bool bReverse = false; // 프레임 역순

    switch (m_eTransitionState)
    {
    case CROSSHAIR_TRANSITION::DEFAULT_TO_HOVER:
        pFrames = &m_vecHoverAnim;
        break;

    case CROSSHAIR_TRANSITION::HOVER_TO_HOLD:
        pFrames = &m_vecHoldAnim;
        break;

    case CROSSHAIR_TRANSITION::HOLD_TO_HOVER:
        pFrames = &m_vecHoldAnim;
        bReverse = true;
        break;

    case CROSSHAIR_TRANSITION::HOVER_TO_DEFAULT:
        pFrames = &m_vecHoverAnim;
        bReverse = true;
        break;

    default:
        if (m_ePrevState == CROSSHAIR_STATE::CROSS_HOVER && m_eState == CROSSHAIR_STATE::CROSS_HOLD)
            pFrames = &m_vecHoldAnim;
        else if (m_ePrevState == CROSSHAIR_STATE::CROSS_HOLD && m_eState == CROSSHAIR_STATE::CROSS_HOVER)
        {
            pFrames = &m_vecHoldAnim;
            bReverse = true;
        }
        else if (m_ePrevState == CROSSHAIR_STATE::CROSS_DEFAULT && m_eState == CROSSHAIR_STATE::CROSS_HOVER)
            pFrames = &m_vecHoverAnim;
        else if (m_ePrevState == CROSSHAIR_STATE::CROSS_HOVER && m_eState == CROSSHAIR_STATE::CROSS_DEFAULT)
        {
            pFrames = &m_vecHoverAnim;
            bReverse = true;
        }
        break;
    }

    if (!pFrames)
        return;

    size_t frameCount = pFrames->size();
    if (m_iAnimFrame >= frameCount)
    {
        m_bAnimating = false;

        if (m_eTransitionState == CROSSHAIR_TRANSITION::DEFAULT_TO_HOVER)
        {
            m_eState = CROSSHAIR_STATE::CROSS_HOVER;
            m_eTransitionState = CROSSHAIR_TRANSITION::HOVER_TO_HOLD;
            m_iAnimFrame = 0;
            m_bAnimating = true;
        }
        else if (m_eTransitionState == CROSSHAIR_TRANSITION::HOVER_TO_HOLD)
        {
            m_eState = CROSSHAIR_STATE::CROSS_HOLD;
            m_eTransitionState = CROSSHAIR_TRANSITION::NONE;
        }
        else if (m_eTransitionState == CROSSHAIR_TRANSITION::HOLD_TO_HOVER)
        {
            m_eState = CROSSHAIR_STATE::CROSS_HOVER;
            m_eTransitionState = CROSSHAIR_TRANSITION::HOVER_TO_DEFAULT;
            m_iAnimFrame = 0;
            m_bAnimating = true;
        }
        else if (m_eTransitionState == CROSSHAIR_TRANSITION::HOVER_TO_DEFAULT)
        {
            m_eState = CROSSHAIR_STATE::CROSS_DEFAULT;
            m_eTransitionState = CROSSHAIR_TRANSITION::NONE;
        }

        return;
    }

    size_t frameIdx = bReverse ? (frameCount - 1 - m_iAnimFrame) : m_iAnimFrame;
    Get_Component<CUiImage>()->Set_Texture((*pFrames)[frameIdx]);

    ++m_iAnimFrame;
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
    m_vecHoverAnim.clear();
    m_vecHoldAnim.clear();
    CGameObject::Free();
}

REGISTER_GAMEOBJECT(CCrosshairUIObject)