#include "CRigidBody.h"
#include "CTransform.h"
#include "CGameObject.h"

CRigidBody::CRigidBody(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
}

CRigidBody::CRigidBody(const CRigidBody& rhs)
    : CComponent(rhs)
    , m_fMass(rhs.m_fMass)
    , m_vVel(rhs.m_vVel)
    , m_vAcc(rhs.m_vAcc)
    , m_vGforce(rhs.m_vGforce)
    , m_bGravity(rhs.m_bGravity)
    , m_pTransform(rhs.m_pTransform)
{
}

CRigidBody::~CRigidBody()
{
}

CRigidBody* CRigidBody::Create(LPDIRECT3DDEVICE9 pGraphicDev, CTransform* pTransform)
{
    CRigidBody* pRigid = new CRigidBody(pGraphicDev);
    pRigid->m_pTransform = pTransform;

    if (pRigid == nullptr || pTransform == nullptr)
    {
        MSG_BOX("Rigidbody Create Failed");
        return nullptr;
    }

    return pRigid;
}

void CRigidBody::Update_Component(const _float& fDeltaTime)
{
    if (m_pTransform == nullptr)
    {
        MSG_BOX("CRigidBody::Update_Component - m_pTransform is null.");
        return;
    }

    // m_bGround = false; // 중력관련 로직 적용 안될 시 

    // 중력 적용
    if (m_bGravity && !m_bGround)
    {
        m_vGforce = _vec3(0.f, -9.8f * m_fGravity, 0.f) * m_fMass;
    }
    else
    {
        m_vGforce = _vec3(0.f, 0.f, 0.f);
    }

    // 외력 + 중력
    _vec3 totalForce = m_vEforce + m_vGforce;

    if (m_fMass > 0.f)
    {
        m_vAcc = totalForce / m_fMass;
        m_vVel += m_vAcc * fDeltaTime;
    }


    // 최대 낙하 속도 제한
    if (m_vVel.y < -50.f)
        m_vVel.y = -50.f;



    // 위치 업데이트
    _vec3 vPos = m_pTransform->Get_Pos();
    vPos += m_vVel * fDeltaTime;
    m_pTransform->Set_Pos(vPos);

    // 회전 반영
    m_vAAcc = { m_vTorque.x / m_fInertia.x ,m_vTorque.y / m_fInertia.y ,m_vTorque.z / m_fInertia.z};
    m_vAVel += m_vAAcc * fDeltaTime;
    if (D3DXVec3LengthSq(&m_vAVel) > 0.f)
    {
        _vec3 axis = m_vAVel;
        float angle = D3DXVec3Length(&axis) * fDeltaTime;

        D3DXVec3Normalize(&axis, &axis);
        m_pTransform->Rotate_Axis(axis, angle);
    }

    // 감쇠 및 초기화
    m_vAVel *= 0.995f;
    m_vTorque = _vec3(0.f, 0.f, 0.f);
    m_vEforce = _vec3(0.f, 0.f, 0.f);


}

void CRigidBody::Free()
{
    Safe_Release(m_pGraphicDev);
    m_pTransform = nullptr;

    //Safe_Release(m_pOwner);
}