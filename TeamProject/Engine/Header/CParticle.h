#pragma once
#include "CComponent.h"

BEGIN(Engine)

enum PARTICLE_MOVE_TYPE
{
    FIRE,
    DUST
};

struct Particle
{
    _vec3 vPos;
    _vec3 vVelocity;
    PARTICLE_MOVE_TYPE moveType;
    float fLifeTime;
    float fAge;
    float fSize;
    D3DCOLOR color;
    bool bActive;
};

class CTexture;


class ENGINE_DLL CParticle :
    public CComponent
{
private:
    explicit CParticle(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CParticle();
public:
    static CParticle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
public:
    HRESULT Ready_Component() override;
    void Update_Component(const _float& fTimeDelta) override;
    void LateUpdate_Component(const _float& fTimeDelta) override;
    void Render_Particle();
    void Emit_Particle();
public:
    float Get_Spawn() { return m_fSpawnInterval; };
    float Get_Size() { return m_fSize; };
    int Get_MaxParticle() { return m_iMaxParticles; };
    int Get_LifeTime() { return m_fLifeTime; };
    int Get_Type() { return m_MoveType; };
    _vec3 Get_Offset() { return m_vOffset; };
    wstring Get_Texture();
    D3DCOLOR Get_Color() { return m_BaseColor; };

public:
    void Set_Spawn(float spawn) {  m_fSpawnInterval = spawn; };
    void Set_Size(float size) {  m_fSize = size; };
    void Set_MaxParticle(int count) {  m_iMaxParticles = count; };
    void Set_LifeTime(float lifeTime) {  m_fLifeTime = lifeTime; };
    void Set_Type(int type) { m_MoveType = static_cast<PARTICLE_MOVE_TYPE>(type); };
    void Set_Offset(_vec3 offSet) {  m_vOffset = offSet; };
    void Set_Color(D3DCOLOR color) {  m_BaseColor = color; };
    void Set_Texture(const wstring& key);

private:
    float randRange(float min, float max);

private:
    //파티클 컴포넌트 설정
    vector<Particle> m_vecParticles;     // 파티클 리스트
    float m_fSpawnInterval = 2.15f;       // 생성 주기
    float m_fElapsedTime = 0.f;
    int m_iMaxParticles = 100;            // 최대 파티클 수

    //파티클 공통 설정
    float m_fLifeTime = 3.5f;
    float m_fSize = 1.5f;
    _vec3 m_vPos = { 0.f,0.f,0.f };
    _vec3 m_vOffset = { 0.f,0.f,0.f };

    CTexture* m_pTexture = nullptr;

    D3DCOLOR m_BaseColor = D3DCOLOR_ARGB(255, 255, 160, 100); // 밝은 주황색
    LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
    PARTICLE_MOVE_TYPE m_MoveType;
    int m_iMovementType = 0;

private:
    void Free() override;
};

END