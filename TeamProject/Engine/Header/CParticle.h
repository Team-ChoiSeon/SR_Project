#pragma once
#include "CComponent.h"

BEGIN(Engine)

enum PARTICLE_MOVE_TYPE
{
    FIRE, DUST, BREATH, RADIAL, SPREAD
};

enum PARTICLE_STATE
{
    EMITTING, STOPPING, NONEACTIVE,
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
    float Get_Speed() { return m_fSpeed; };

    int Get_MaxParticle() { return m_iMaxParticles; };
    int Get_LifeTime() { return m_fLifeTime; };
    int Get_Type() { return m_eMoveType; };
    _vec3 Get_Offset() { return m_vOffset; };

    wstring Get_Texture();
    D3DCOLOR Get_Color() { return m_BaseColor; };

public:
    void Set_Type(PARTICLE_MOVE_TYPE type) { m_eMoveType = type; };

    void Set_Color(D3DCOLOR color) { m_BaseColor = color; };
    void Set_Texture(const wstring& key);

    void Set_Speed(float speed) { m_fSpeed = speed; };
    void Set_Offset(_vec3 offSet) { m_vOffset = offSet; };
    void Set_SpawnInterval(float spawn) { m_fSpawnInterval = spawn; };
    void Set_MaxParticle(int count) { m_iMaxParticles = count; };
    void Set_Size(float size) { m_fSize = size; m_vecParticles.resize(size); };
    void Set_LifeTime(float lifeTime) { m_fLifeTime = lifeTime; };
    void Request_Emit();

public:
    // Pre-configurations for particle types
    void PreSet_Dust(int MaxCount, float Interval, float lifeTime);
    void PreSet_Fire(int MaxCount, float Interval, float lifeTime);
    void PreSet_Breath(_vec3 dir , int MaxCount, float Interval, float lifeTime, float speed);
    void PreSet_Radial(int MaxCount, float lifeTime);
    void PreSet_Spread(int MaxCount, float Interval, float lifeTime, float speed);
    void PreSet_Loop(_bool loop);
    void PreSet_Single(float EmitTime);

private:
    float randRange(float min, float max);
    int Particle_Count();

private:
    int m_iTextureIndex = 0;

    // Particle system configuration (shared across all particles)
    vector<Particle> m_vecParticles;     // Particle list
    float m_fSpawnInterval = 0.5f;       // Spawn interval
    float m_fElapsedTime = 0.f;
    int m_iMaxParticles = 100;           // Maximum number of particles
    float m_fSpeed = 1.f;                // Particle movement speed
    float m_fLifeTime = 3.5f;
    float m_fSize = 1.5f;

    _vec3 m_vVelocity = { 0.f,0.f,0.f };  // Direction of movement
    _vec3 m_vPos = { 0.f,0.f,0.f };       // Calculated from owner's transform
    _vec3 m_vOffset = { 0.f,0.f,0.f };    // Offset from owner's position
    D3DCOLOR m_BaseColor = D3DCOLOR_ARGB(255, 255, 160, 100); // Bright orange

    _bool m_bLoop;                       // Looping enabled?
    float m_fEmitLife = 3.5f;            // Duration to emit when not looping
    float m_fElapsedEmit = 0.f;          // Time passed since emission started

    // For radial/spread movement
    int m_EmitCount = 0;                 // How many particles to emit at once
    float m_fAngle = 0;                  // Current angle for radial emission

    LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;

    PARTICLE_MOVE_TYPE m_eMoveType = DUST;
    PARTICLE_STATE m_eNowState = EMITTING;

    CTexture* m_pTexture = nullptr;

private:
    void Free() override;
};

END
