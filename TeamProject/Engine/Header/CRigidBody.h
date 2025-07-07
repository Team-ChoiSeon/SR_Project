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
    void Set_Friction(_float fFric) { m_fFric = fFric; }
    void Set_Bounce(_float fBnc) { m_fBnc = fBnc; }

    void Add_Force(const _vec3& vForce) { m_vEforce += vForce; }
    void Add_Torque(const _vec3& vTorque) { m_vTorque += vTorque; }
    void Add_Velocity(const _vec3& vVel) { m_vVel += vVel; }

    void Set_Force(const _vec3& vForce) { m_vEforce = vForce; }
    void Set_Torque(const _vec3& vTorque){m_vTorque = vTorque;}
    void Set_Velocity(const _vec3& vVel) { m_vVel = vVel; }
    void Set_AVelocity(const _vec3& vAVel){ m_vAVel = vAVel;}

    bool  Get_OnGround()   const     { return m_bGround; }
    bool  Get_UseGravity() const     { return m_bGravity; }

    _float Get_Mass()   const { return m_fMass; }
    _vec3 Get_Velocity()   const     { return m_vVel; }
    _vec3 Get_AVelocity()  const     { return m_vAVel; }
    _vec3 Get_Accel()      const     { return m_vAcc; }
    _vec3 Get_Gforce()     const     { return m_vGforce; }
    _vec3 Get_Eforce()     const     { return m_vEforce; }

    void Update_Component(const _float& fDeltaTime) override;

private:
    virtual void Free() override;

private:
    _float      m_fMass = 1.f;                          // 질량
    _float      m_fFric = 0.f;                     // 마찰 계수
    _float      m_fBnc = 0.0f;                    // 탄성 계수
    
    _vec3       m_vVel = _vec3(0.f, 0.f, 0.f);     // 속도
    _vec3       m_vAcc = _vec3(0.f, 0.f, 0.f);        // 가속도
    _vec3       m_vAVel = _vec3(0.f, 0.f, 0.f);    // 각속도
    _vec3       m_vAAcc = _vec3(0.f, 0.f, 0.f);    // 각가속도

    _vec3       m_vGforce = _vec3(0.f, 0.f, 0.f);       // 중력
    _vec3       m_vEforce = _vec3(0.f, 0.f, 0.f);       // 외부 힘
    _vec3       m_vTorque = _vec3(0.f, 0.f, 0.f);       // 회전 힘
    _vec3       m_fInertia = _vec3(1.f, 1.f, 1.f);      // 관성모먼트

    bool        m_bGravity = true;
    bool        m_bGround = false;

    CTransform* m_pTransform = nullptr;
};

END