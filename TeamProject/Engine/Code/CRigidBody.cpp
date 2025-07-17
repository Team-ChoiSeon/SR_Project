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

    // �߷� ����
    if (m_bGravity && !m_bGround)
    {
        m_vGforce = _vec3(0.f, -9.8f * m_fGravity, 0.f) * m_fMass;
    }
    else
    {
        m_vGforce = _vec3(0.f, 0.f, 0.f);
    }

    // �ܷ� + �߷�
    _vec3 totalForce = m_vEforce + m_vGforce;

    //if (m_bGround && D3DXVec3LengthSq(&m_vVel) > 0.0001f)
    //{
    //    // � ������ ���: N(�����׷�) * u(�������)
    //    // ���������� �����׷��� �߷��� ũ��� ���ٰ� �����մϴ�.
    //    float fNormalForce = m_fMass * 9.8f * m_fGravity;
    //    float fFrictionMagnitude = fNormalForce * m_fFric;

    //    // ������ ������ �ӵ��� �ݴ� ����
    //    _vec3 vFrictionForce = -m_vVel;
    //    vFrictionForce.y = 0.f; // ���� ������ ����
    //    D3DXVec3Normalize(&vFrictionForce, &vFrictionForce);

    //    vFrictionForce *= fFrictionMagnitude;

    //    // �������� ���� �ӵ��� �Ѿ� �ݴ� �������� �����̰� �ϴ� ���� ����
    //    _vec3 vNextVel = m_vVel + (vFrictionForce / m_fMass) * fDeltaTime;
    //    if (D3DXVec3Dot(&m_vVel, &vNextVel) < 0.f)
    //    {
    //        // �ӵ��� 0�� �Ǵ� ������ �Ѿ�� �׳� �ӵ��� 0���� ����
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


    // �ִ� ���� �ӵ� ����
    if (m_vVel.y < -50.f)
        m_vVel.y = -50.f;



    // ��ġ ������Ʈ
    _vec3 vPos = m_pTransform->Get_Pos();
    vPos += m_vVel * fDeltaTime;
    m_pTransform->Set_Pos(vPos);

    // ȸ�� �ݿ�
    m_vAAcc = { m_vTorque.x / m_fInertia.x ,m_vTorque.y / m_fInertia.y ,m_vTorque.z / m_fInertia.z};
    m_vAVel += m_vAAcc * fDeltaTime;
    if (D3DXVec3LengthSq(&m_vAVel) > 0.f)
    {
        _vec3 axis = m_vAVel;
        float angle = D3DXVec3Length(&axis) * fDeltaTime;

        D3DXVec3Normalize(&axis, &axis);
        m_pTransform->Rotate_Axis(axis, angle);
    }

    // ���� �� �ʱ�ȭ
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