#include "CMonsterPart.h"

#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h" 

CMonsterPart::CMonsterPart(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CMonsterPart::CMonsterPart(const CMonsterPart& rhs)
    : CGameObject(rhs.m_pGraphicDev),m_pModel(rhs.m_pModel),m_pTarget(rhs.m_pTarget)
{

}

CMonsterPart::~CMonsterPart()
{
}

CMonsterPart* CMonsterPart::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMonsterPart* pPart = new CMonsterPart(pGraphicDev);

    if (FAILED(pPart->Ready_GameObject()))
    {
        Safe_Release(pPart);
        MSG_BOX("PartObject Create Failed");
        return nullptr;
    }

    return pPart;
}

HRESULT CMonsterPart::Ready_GameObject()
{
    // 1. �� ������Ʈ �߰� �� ��� Ȯ��
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();
    if (nullptr == m_pModel)
    {
        MSG_BOX("Part - CModel creation failed!");
        return E_FAIL;
    }

    // 2. Ʈ������ ������Ʈ �߰� �� ��� Ȯ��
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();
    if (nullptr == m_pTransform)
    {
        MSG_BOX("Part - CTransform creation failed!");
        return E_FAIL;
    }

    // 3. ������ٵ� ������Ʈ �߰� �� ��� Ȯ��
    // ���� m_pTransform�� nullptr�� �ƴ��� ����˴ϴ�.
    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();
    if (nullptr == m_pRigid)
    {
        MSG_BOX("Part - CRigidBody creation failed!");
        return E_FAIL;
    }

    // 4. �ݶ��̴� ������Ʈ �߰� �� ��� Ȯ��
    // ���� m_pRigid�� nullptr�� �ƴ��� ����˴ϴ�.
    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCol = Get_Component<CCollider>();
    if (nullptr == m_pCol)
    {
        MSG_BOX("Part - CCollider creation failed!");
        return E_FAIL;
    }

    // --- ��� ������Ʈ�� ���������� ������ �� ���� �ڵ� ���� ---
    m_pCol->Set_ColTag(ColliderTag::NONE);
    m_pCol->Set_ColType(ColliderType::PASSIVE);
    m_pCol->Set_BoundType(BoundingType::OBB);

    m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

    m_pRigid->Set_OnGround(false);
    m_pRigid->Set_UseGravity(false);
    m_pRigid->Set_Mass(1.f);
    m_pRigid->Set_Friction(1.f);
    m_pRigid->Set_Bounce(0.f);


    return S_OK;
}

_int CMonsterPart::Update_GameObject(const _float& fTimeDelta)
{
    Follow_Target(fTimeDelta);
    CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CMonsterPart::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMonsterPart::Follow_Target(_float fDeltaTime)
{
    if (!m_pTarget || !m_pTransform)
        return;

    // 1. �� ��ġ�� Ÿ���� ��ġ ��������
    const _vec3 myPos = m_pTransform->Get_Pos();
    const _vec3 targetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();

    // 2. Ÿ�ٰ��� ���� �� �Ÿ� ���
    _vec3 dir = myPos - targetPos;
    float dist = D3DXVec3Length(&dir);

    if (dist > 0.001f)
        D3DXVec3Normalize(&dir, &dir);

    // 3. ��ǥ ��ġ ���
    //  Ÿ���� ��ġ���� dir �������� baseDist��ŭ ������ ����
    float baseDist = 2.0f;
    _vec3 movePos = myPos;

    // �о��
    if (dist < baseDist)
    {
        _vec3 pushDir = myPos - targetPos;
        D3DXVec3Normalize(&pushDir, &pushDir);
        // �ڷ� �з����� ���ο� ��ġ ���
        movePos = myPos + pushDir * m_fSpeed * fDeltaTime * 0.66f;
    }

    // ���󰡱�
    else
    {
        _vec3 dirFromTarget = myPos - targetPos;
        D3DXVec3Normalize(&dirFromTarget, &dirFromTarget);
        _vec3 goalPos = targetPos + dirFromTarget * baseDist;
        movePos = myPos + (goalPos - myPos) * m_fSpeed * fDeltaTime;
    }

    m_pTransform->Set_Pos(movePos);

    _vec3 lookDir = targetPos - myPos;
    D3DXVec3Normalize(&lookDir, &lookDir);
    m_pTransform->Set_Look(lookDir);
}


void CMonsterPart::Free()
{
    Safe_Release(m_pModel);
    Safe_Release(m_pTransform);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCol);
    m_pTarget = nullptr;
}
