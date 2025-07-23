#pragma once
#include "CComponent.h"

BEGIN(Engine)

class CTexture;

class ENGINE_DLL CEffect : public CComponent
{
private:
    explicit CEffect(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CEffect();

public:
    static CEffect* Create(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    virtual HRESULT Ready_Component() override;
    virtual void Update_Component(const _float& fTimeDelta) override;
    virtual void LateUpdate_Component(const _float& fTimeDelta) override;
    void Render_Effect();

public:
    // 이펙트 재생을 시작하는 함수 (XZ 평면 좌표 사용)
    void Play(const _vec2& vPosXZ, float fLifeTime);

    // 시각적 표현을 설정하는 함수
    void Set_SpriteSheet(const std::wstring& textureKey, int framesX, int framesY, float speed);

    // 이펙트의 속성을 설정하는 함수
    void Set_EffectProperties(float fSize, _bool bLoop);
    void Set_YOffset(float fOffsetY) { m_fOffsetY = fOffsetY; }
    void Set_Color(D3DCOLOR color) { m_BaseColor = color; }
    void Set_Active(bool active) { m_bIsActive = active; }
    _bool Is_Playing() const { return m_bIsActive; }

private:
    CTexture* m_pTexture = nullptr;
    LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;

    // 이펙트 자체의 상태 변수
    _vec2 m_vPosXZ = { 0.f, 0.f };
    float m_fOffsetY = 0.01f;
    float m_fSize = 1.f;
    float m_fLifeTime = 1.f;
    float m_fAge = 0.f;
    bool m_bIsActive = false;
    bool m_bLoop = false;
    D3DCOLOR m_BaseColor = D3DCOLOR_ARGB(255, 255, 255, 255);

    // UV 애니메이션 정보
    int m_iAnimFramesX = 1;
    int m_iAnimFramesY = 1;
    float m_fAnimationSpeed = 10.f;
    float m_fCurrentFrame = 0.f;

private:
    virtual void Free() override;
};

END

/*

fLifeTime = 총 프레임 수 / speed
사용 예시
16프레임짜리 폭발 애니메이션을 초당 20프레임(speed = 20.f)의 속도로 재생하고 싶을 경우:

애니메이션 재생 시간: 16 / 20.f = 0.8초

설정할 LifeTime: 0.8f

*/