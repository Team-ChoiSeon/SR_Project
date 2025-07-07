#include "CRigidbody.h"
#include "CTransform.h"

USING(Engine)

CRigidbody::CRigidbody(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
}

CRigidbody::CRigidbody(const CRigidbody& rhs)
    : CComponent(rhs)
    , m_fMass(rhs.m_fMass)
    , m_vVelocity(rhs.m_vVelocity)
    , m_vAccel(rhs.m_vAccel)
    , m_vGforce(rhs.m_vGforce)
    , m_bGravity(rhs.m_bGravity)
    , m_pTransform(rhs.m_pTransform)
{
}

CRigidbody::~CRigidbody()
{
}

CRigidbody* CRigidbody::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pTransform)
{
    CRigidbody* pInstance = new CRigidbody(pGraphicDev);
    pInstance->m_pTransform = pTransform;

    if (pInstance == nullptr || pTransform == nullptr)
    {
        MSG_BOX("Rigidbody Create Failed");
        return nullptr;
    }

    return pInstance;
}

void CRigidbody::Add_Force(const _vec3& vForce)
{
    m_vEforce += vForce;
}

void CRigidbody::Add_Velocity(const _vec3& vVel)
{
    m_vVelocity += vVel;
}

void CRigidbody::Update_Component(const _float& fDeltaTime)
{
    if (m_pTransform == nullptr)
        return;

    // 중력 적용
    if (m_bGravity && !m_bGround)
        m_vGforce = _vec3(0.f, -9.8f, 0.f) * m_fMass;
    else
        m_vGforce = _vec3(0.f, 0.f, 0.f);

    _vec3 totalForce = m_vEforce + m_vGforce;
    // F = m * a  =>  a = F / m
    m_vAccel = totalForce / m_fMass;

    // 속도 업데이트
    m_vVelocity += m_vAccel * fDeltaTime;

    if (m_bGround && m_vVelocity.y < 0.f)
    {
        m_vVelocity.y = 0.f;
        m_vAccel.y = 0.f;
    }

    // 위치 업데이트
    _vec3 vPos = m_pTransform->Get_Pos();
    vPos += m_vVelocity * fDeltaTime;
    m_pTransform->Set_Pos(vPos);

    // 힘 초기화
    m_vEforce = _vec3(0.f, 0.f, 0.f);

}

void CRigidbody::Free()
{
    Safe_Release(m_pGraphicDev);
    m_pTransform = nullptr;
}