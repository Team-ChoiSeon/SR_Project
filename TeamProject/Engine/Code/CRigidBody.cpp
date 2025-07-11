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

    // 마찰력 : 지면 위에서만
    if (m_bGround)
    {
        _vec3 friction = -m_vVel;
        friction.y = 0.f; // y축 마찰 무시 (점프/낙하 방해 방지)
        friction *= m_fFric;
        totalForce += friction;
    }


    if (m_fMass == 0.f) return;
    // F = m * a  =>  a = F / m
    m_vAcc = totalForce / m_fMass;
    // 속도 업데이트
    m_vVel += m_vAcc * fDeltaTime;

    // 지면에서 미세한 움직임 컷팅
    if (m_bGround)
    {
        if (D3DXVec3LengthSq(&m_vVel) < 0.001f)
            m_vVel = _vec3(0.f, 0.f, 0.f);

        if (D3DXVec3LengthSq(&m_vAcc) < 0.01f)
            m_vAcc = _vec3(0.f, 0.f, 0.f);
    }

    // 낙하 처리 : 탄성 적용
    if (m_bGround && m_vVel.y < 0.f)
    {
        m_vVel.y *= -m_fBnc;

        if (fabs(m_vVel.y) < 0.05f)
        {
            m_vVel.y = 0.f;
            m_vAcc.y = 0.f;
        }
        else
        {
            // 여기서 튕겨 오르면 지면에서 떨어진 상태이므로 false 처리
            m_bGround = false;
        }
    }
    else if (m_bGround)
    {
        // 땅 위에 충분히 닿았고 속도가 작다면 확실히 고정
        m_vVel.y = 0.f;
        m_vAcc.y = 0.f;
    }

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