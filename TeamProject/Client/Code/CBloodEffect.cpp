#include "pch.h"
#include "Engine_Define.h"
#include "CBloodEffect.h"
#include "CUiQuad.h"
#include "CTransform.h"

CBloodEffect::CBloodEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CBloodEffect::~CBloodEffect()
{
}

CBloodEffect* CBloodEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CBloodEffect* instance = new CBloodEffect(pGraphicDev);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CBloodEffect::Ready_GameObject()
{
    m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);

    m_tPanel.Set_Size({ 30,30 });

    m_pQuad->Set_Texture(L"UI/Blood_Effect.png");
    m_pQuad->Set_Shader(L"g_UIShader.fx");

    return S_OK;
}

_int CBloodEffect::Update_GameObject(const _float& fTimeDelta)
{
    if (!m_bActive) return 0;
    m_fElapsedTime += fTimeDelta;

    if (m_fDurateTime < m_fElapsedTime) {
        m_fElapsedTime = 0;
        m_bActive = false;
    }

        m_tPanel.Add_Size({ -fTimeDelta,-fTimeDelta });
        m_pQuad->Set_Alpha(m_pQuad->Get_Alpha() - fTimeDelta);

    CGameObject::Update_GameObject(fTimeDelta);
    m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
    m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
    return 0;
}

void CBloodEffect::LateUpdate_GameObject(const _float& fTimeDelta)
{
    if (!m_bActive) return;
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CBloodEffect::Active_Blood(float duration)
{
    m_bActive = true;
    m_pQuad->Set_Alpha(0.3);
    m_fDurateTime = duration;
}

void CBloodEffect::SetRand_Transform()
{
    _vec2 pos = { static_cast<_float>(rand() % WINCX), static_cast<_float>(rand() % WINCY)};
    float scale = static_cast<_float>(rand() % 300);

    m_tPanel.Set_Pos(pos);
    m_tPanel.Set_Size({ scale ,scale });
}

void CBloodEffect::Free()
{
    CGameObject::Free();
}
