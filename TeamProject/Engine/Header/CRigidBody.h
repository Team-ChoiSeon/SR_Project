#pragma once
#include "CComponent.h"

BEGIN(Engine)

class CTransform;

class ENGINE_DLL CRigidbody : public CComponent
{
private:
    explicit CRigidbody(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CRigidbody(const CRigidbody& rhs);
    virtual ~CRigidbody();

public:
    static CRigidbody* Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pTransform);

public:
    void Set_OnGround(bool bGround) { m_bGround = bGround; }
    void Set_UseGravity(bool bUse) { m_bGravity = bUse; }
    void Set_Mass(_float fMass) { m_fMass = fMass; }
    void Add_Force(const _vec3& vForce);
    void Add_Velocity(const _vec3& vVel);
    void Set_Velocity(const _vec3& vVel) { m_vVelocity = vVel; }

    _vec3 Get_Velocity() const { return m_vVelocity; }
    _vec3 Get_Accel() const { return m_vAccel; }
    _vec3 Get_Gforce() const { return m_vGforce; }
    _vec3 Get_Eforce() const { return m_vGforce; }

    void Update_Component(const _float& fDeltaTime) override;

private:
    virtual void Free() override;

private:
    _float      m_fMass = 1.f;                          // 질량
    _vec3       m_vVelocity = _vec3(0.f, 0.f, 0.f);     // 속도
    _vec3       m_vAccel = _vec3(0.f, 0.f, 0.f);        // 가속도
    _vec3       m_vGforce = _vec3(0.f, 0.f, 0.f);       // 중력
    _vec3       m_vEforce = _vec3(0.f, 0.f, 0.f);       // 외부 힘

    bool        m_bGravity = true;
    bool        m_bGround = false;

    CTransform* m_pTransform = nullptr;
};

END