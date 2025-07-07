#include "CRigidbody.h"
#include "CTransform.h"


CRigidbody::CRigidbody(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev)
{
}

CRigidbody::CRigidbody(const CRigidbody& rhs)
    : CComponent(rhs)
    , m_fMass(rhs.m_fMass)
    , m_vVel(rhs.m_vVel)
    , m_vAcc(rhs.m_vAcc)
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

void CRigidbody::Update_Component(const _float& fDeltaTime)
{
    if (m_pTransform == nullptr)
        return;

    // �߷� ����
    if (m_bGravity && !m_bGround)
        m_vGforce = _vec3(0.f, -9.8f, 0.f) * m_fMass;
    else
        m_vGforce = _vec3(0.f, 0.f, 0.f);

    // �ܷ� + �߷�
    _vec3 totalForce = m_vEforce + m_vGforce;

    // ������ : ���� ��������
    if (m_bGround)
    {
        _vec3 friction = -m_vVel;
        friction.y = 0.f; // y�� ���� ���� (����/���� ���� ����)
        friction *= m_fFric;
        totalForce += friction;
    }

    // F = m * a  =>  a = F / m
    m_vAcc = totalForce / m_fMass;
    // �ӵ� ������Ʈ
    m_vVel += m_vAcc * fDeltaTime;

    // �̼��� ������ ����
    if (D3DXVec3Length(&m_vVel) < 0.001f)
        m_vVel = _vec3(0.f, 0.f, 0.f);

    // ���� ó�� : ź�� ����
    if (m_bGround && m_vVel.y < 0.f)
    {
        m_vVel.y *= -m_fBnc;

        if (fabs(m_vVel.y) < 0.05f) // �ʹ� ������ ����
        {
            m_vVel.y = 0.f;
        }
        else
        {
            // ���⼭ ƨ�� ������ ���鿡�� ������ �����̹Ƿ� false ó��
            m_bGround = false;
        }
    }

    // ��ġ ������Ʈ
    _vec3 vPos = m_pTransform->Get_Pos();
    vPos += m_vVel * fDeltaTime;
    m_pTransform->Set_Pos(vPos);

    //m_vAAcc = m_vTorque / m_fInertia;
    m_vAVel += m_vAAcc * fDeltaTime;
    // �� �ʱ�ȭ
    m_vEforce = _vec3(0.f, 0.f, 0.f);

}

void CRigidbody::Free()
{
    Safe_Release(m_pGraphicDev);
    m_pTransform = nullptr;
}