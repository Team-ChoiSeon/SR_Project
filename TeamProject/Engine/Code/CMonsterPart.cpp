#include "CMonsterPart.h"

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
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);

    m_pTransform = Get_Component<CTransform>();
    m_pModel = Get_Component<CModel>();

    m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

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

    const _vec3 myPos = m_pTransform->Get_Pos();
    const _vec3 targetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();

    _vec3 dir = targetPos - myPos;
    float dist = D3DXVec3Length(&dir);
    if (dist < 0.01f)
        return;

    D3DXVec3Normalize(&dir, &dir);
    float speed = 5.f;
    _vec3 newPos = myPos + dir * fDeltaTime * speed;

    m_pTransform->Set_Pos(newPos);
}


void CMonsterPart::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pModel);
    m_pTarget = nullptr;
}