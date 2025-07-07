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
    _vec3 Get_Force() const { return m_vForce; }

    void Update_Component(const _float& fDeltaTime) override;

private:
    virtual void Free() override;

private:
    _float      m_fMass = 1.f;
    _vec3       m_vVelocity = _vec3(0.f, 0.f, 0.f);
    _vec3       m_vAccel = _vec3(0.f, 0.f, 0.f);
    _vec3       m_vForce = _vec3(0.f, 0.f, 0.f);

    bool        m_bGravity = true;
    bool        m_bGround = false;

    CTransform* m_pTransform = nullptr;
};

END