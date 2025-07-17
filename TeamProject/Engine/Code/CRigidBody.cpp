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

    if (m_bGround)
    {
        // 현재 속도에서 수평 방향 성분만 추출
        _vec3 vHorizontalVel = m_vVel;
        vHorizontalVel.y = 0.f;

        // 수평 속도가 조금이라도 있을 경우 (미끄러지고 있을 경우)
        if (D3DXVec3LengthSq(&vHorizontalVel) > 0.0001f)
        {
            // 마찰력의 방향은 속도의 정반대
            _vec3 vFrictionDir;
            D3DXVec3Normalize(&vFrictionDir, &vHorizontalVel);
            vFrictionDir *= -1.f;

            // 마찰력의 크기 계산 (간단한 모델)
            // 실제로는 수직항력(Normal Force)을 곱해야 하지만, 여기서는 중력을 기반으로 근사치 계산
            float fNormalForce = m_fMass * 9.8f;
            float fFrictionMagnitude = fNormalForce * m_fFric;

            // 마찰 계수가 적용된 최종 마찰력
            _vec3 vFrictionForce = vFrictionDir * fFrictionMagnitude;

            // 최종 힘에 마찰력을 더해줌
            totalForce += vFrictionForce;
        }
    }

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