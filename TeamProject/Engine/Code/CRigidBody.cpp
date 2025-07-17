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

    //m_bGround = false;

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

    //if (m_bGround && D3DXVec3LengthSq(&m_vVel) > 0.0001f)
    //{
    //    // 운동 마찰력 계산: N(수직항력) * u(마찰계수)
    //    // 평지에서의 수직항력은 중력의 크기와 같다고 가정합니다.
    //    float fNormalForce = m_fMass * 9.8f * m_fGravity;
    //    float fFrictionMagnitude = fNormalForce * m_fFric;

    //    // 마찰력 방향은 속도의 반대 방향
    //    _vec3 vFrictionForce = -m_vVel;
    //    vFrictionForce.y = 0.f; // 수평 마찰만 적용
    //    D3DXVec3Normalize(&vFrictionForce, &vFrictionForce);

    //    vFrictionForce *= fFrictionMagnitude;

    //    // 마찰력이 현재 속도를 넘어 반대 방향으로 움직이게 하는 것을 방지
    //    _vec3 vNextVel = m_vVel + (vFrictionForce / m_fMass) * fDeltaTime;
    //    if (D3DXVec3Dot(&m_vVel, &vNextVel) < 0.f)
    //    {
    //        // 속도가 0이 되는 지점을 넘어서면 그냥 속도를 0으로 만듦
    //        m_vVel.x = 0.f;
    //        m_vVel.z = 0.f;
    //    }
    //    else
    //    {
    //        totalForce += vFrictionForce;
    //    }
    //}

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