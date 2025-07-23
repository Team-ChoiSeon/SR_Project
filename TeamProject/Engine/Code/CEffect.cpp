#include "CEffect.h"
#include "CRenderMgr.h"
#include "CResourceMgr.h"
#include "CTexture.h"

#include "CTransform.h"

// FVF 및 Vertex 구조체 선언 (엔진의 다른 곳에 정의되어 있어야 함)
// 예시:
struct VTXPARTICLE { _vec3 vPos; D3DCOLOR dwColor; _vec2 vTexUV; };
const _ulong FVF_PARTICLE = D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1;


BEGIN(Engine)

CEffect::CEffect(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
}

CEffect::~CEffect()
{
}

CEffect* CEffect::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CEffect* pInstance = new CEffect(pGraphicDev);
    if (FAILED(pInstance->Ready_Component()))
    {
        MSG_BOX("CEffect Create Failed");
        Safe_Release(pInstance);
    }
    return pInstance;
}

HRESULT CEffect::Ready_Component()
{
    // 정점 6개짜리 버텍스 버퍼를 생성합니다.
    HRESULT hr = m_pGraphicDev->CreateVertexBuffer(
        sizeof(VTXPARTICLE) * 6,
        D3DUSAGE_DYNAMIC | D3DUSAGE_WRITEONLY,
        FVF_PARTICLE,
        D3DPOOL_DEFAULT,
        &m_pVB,
        nullptr);

    if (hr == E_FAIL)  return E_FAIL;

    return S_OK;
}

void CEffect::Update_Component(const _float& fTimeDelta)
{
    if (!m_bIsActive)
        return;

    // 1. 수명 계산
    m_fAge += fTimeDelta;
    // 2. 애니메이션 프레임 계산
    m_fCurrentFrame += m_fAnimationSpeed * fTimeDelta;

    // 3. 수명이 다했는지 체크
    if (m_fAge >= m_fLifeTime)
    {
        if (m_bLoop) // 반복 모드이면 처음부터 다시 시작
        {
            m_fAge = 0.f;
            m_fCurrentFrame = 0.f;
        }
        else // 반복하지 않으면 비활성화
        {
            m_bIsActive = false;
        }
    }
}

void CEffect::LateUpdate_Component(const _float& fTimeDelta)
{
    // 활성화 상태일 때만 렌더링 매니저에 등록합니다.
    if (m_bIsActive)
        CRenderMgr::Get_Instance()->Add_Effect(this); // 렌더 매니저에 이펙트 렌더러 목록 추가 기능 필요
}

void CEffect::Play(const _vec2& vPosXZ, float fLifeTime)
{
    m_vPosXZ = vPosXZ;         // 위치를 이 때 한 번만 설정
    m_fLifeTime = fLifeTime;   // 수명을 이 때 설정
    m_fAge = 0.f;
    m_fCurrentFrame = 0.f;
    m_bIsActive = true;
}

void CEffect::Set_SpriteSheet(const std::wstring& textureKey, int framesX, int framesY, float speed)
{
    m_pTexture = CResourceMgr::Get_Instance()->Load_Texture(textureKey);
    m_iAnimFramesX = max(1, framesX);
    m_iAnimFramesY = max(1, framesY);
    m_fAnimationSpeed = speed;
}

void CEffect::Set_EffectProperties(float fSize, _bool bLoop)
{
    m_fSize = fSize;
    m_bLoop = bLoop;
}

void CEffect::Render_Effect()
{
    if (!m_pGraphicDev || !m_pTexture || !m_pVB || !m_bIsActive)
        return;
    // 렌더 상태 저장 (복원용)
    DWORD dwZWriteEnable, dwAlphaBlendEnable, dwCullMode;
    m_pGraphicDev->GetRenderState(D3DRS_ZWRITEENABLE, &dwZWriteEnable);
    m_pGraphicDev->GetRenderState(D3DRS_ALPHABLENDENABLE, &dwAlphaBlendEnable);
    m_pGraphicDev->GetRenderState(D3DRS_CULLMODE, &dwCullMode);

    DWORD dwColorOp, dwAlphaOp;
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_COLOROP, &dwColorOp);
    m_pGraphicDev->GetTextureStageState(0, D3DTSS_ALPHAOP, &dwAlphaOp);
    // 렌더 상태 설정
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, FALSE);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    m_pGraphicDev->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);


    m_pTexture->Bind(m_pGraphicDev, 0);
    m_pGraphicDev->SetFVF(FVF_PARTICLE);

    VTXPARTICLE* pVertices = nullptr;
    if (FAILED(m_pVB->Lock(0, 0, (void**)&pVertices, D3DLOCK_DISCARD)))
        return;

    // 3D 중심점 계산
    const _vec3 c = { m_vPosXZ.x, m_fOffsetY, m_vPosXZ.y };
    const float half = m_fSize * 0.5f;

    // UV 애니메이션 좌표 계산
    const int totalFrames = m_iAnimFramesX * m_iAnimFramesY;
    int iCurrentFrame = static_cast<int>(m_fCurrentFrame);

    if (m_bLoop)
        iCurrentFrame %= totalFrames;
    else
        iCurrentFrame = min(iCurrentFrame, totalFrames - 1);

    const float fFrameWidth = 1.f / m_iAnimFramesX;
    const float fFrameHeight = 1.f / m_iAnimFramesY;
    const float u = (iCurrentFrame % m_iAnimFramesX) * fFrameWidth;
    const float v = (iCurrentFrame / m_iAnimFramesX) * fFrameHeight;

    // 고정된 XZ 평면의 정점 위치 계산
    _vec3 v1 = { c.x - half, c.y, c.z - half }; // 좌하
    _vec2 uv1 = { u, v + fFrameHeight };

    _vec3 v2 = { c.x - half, c.y, c.z + half }; // 좌상
    _vec2 uv2 = { u, v };

    _vec3 v3 = { c.x + half, c.y, c.z + half }; // 우상
    _vec2 uv3 = { u + fFrameWidth, v };

    _vec3 v4 = { c.x + half, c.y, c.z - half }; // 우하
    _vec2 uv4 = { u + fFrameWidth, v + fFrameHeight };

    D3DCOLOR color = m_BaseColor;

    // 버텍스 버퍼 채우기 (삼각형 2개)
    pVertices[0] = { v2, color, uv2 };
    pVertices[1] = { v3, color, uv3 };
    pVertices[2] = { v4, color, uv4 };

    pVertices[3] = { v2, color, uv2 };
    pVertices[4] = { v4, color, uv4 };
    pVertices[5] = { v1, color, uv1 };

    m_pVB->Unlock();

    // 그리기 및 렌더 상태 복구
    m_pGraphicDev->SetStreamSource(0, m_pVB, 0, sizeof(VTXPARTICLE));
    m_pGraphicDev->DrawPrimitive(D3DPT_TRIANGLELIST, 0, 2);


    m_pGraphicDev->SetTexture(0, nullptr);
    m_pGraphicDev->SetRenderState(D3DRS_ZWRITEENABLE, dwZWriteEnable);
    m_pGraphicDev->SetRenderState(D3DRS_ALPHABLENDENABLE, dwAlphaBlendEnable);
    m_pGraphicDev->SetRenderState(D3DRS_CULLMODE, dwCullMode);

    m_pGraphicDev->SetTextureStageState(0, D3DTSS_COLOROP, dwColorOp);
    m_pGraphicDev->SetTextureStageState(0, D3DTSS_ALPHAOP, dwAlphaOp);
}

void CEffect::Free()
{
    Safe_Release(m_pVB);
}

END