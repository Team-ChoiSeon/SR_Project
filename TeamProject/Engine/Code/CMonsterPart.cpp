#include "CMonsterPart.h"

#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h" 
#include "CParticle.h"


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
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();
    if (nullptr == m_pModel)
    {
        MSG_BOX("Part - CModel creation failed!");
        return E_FAIL;
    }

    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();
    if (nullptr == m_pTransform)
    {
        MSG_BOX("Part - CTransform creation failed!");
        return E_FAIL;
    }

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();
    if (nullptr == m_pRigid)
    {
        MSG_BOX("Part - CRigidBody creation failed!");
        return E_FAIL;
    }

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCol = Get_Component<CCollider>();
    if (nullptr == m_pCol)
    {
        MSG_BOX("Part - CCollider creation failed!");
        return E_FAIL;
    }

    Add_Component<CParticle>(ID_DYNAMIC, m_pGraphicDev);
    m_pParticle = Get_Component<CParticle>();
    m_pParticle->Set_Texture(L"vecteezy_smoke-effect-transparent_21104616.png");
    m_pParticle->Set_Type(PARTICLE_MOVE_TYPE::BREATH);
    m_pParticle->Set_MaxParticle(200);
    m_pParticle->Set_SpawnInterval(0.1f);

    m_pCol->Set_ColTag(ColliderTag::MONSTER);
    m_pCol->Set_ColType(ColliderType::PASSIVE);
    m_pCol->Set_BoundType(BoundingType::OBB);

    m_pTransform->Set_Scale({ 2.f, 2.f, 2.f });

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

    const _vec3 myPos = m_pTransform->Get_Pos();
    const _vec3 targetPos = m_pTarget->Get_Component<CTransform>()->Get_Pos();

    _vec3 dir = myPos - targetPos;
    float dist = D3DXVec3Length(&dir);

    if (dist > 0.001f)
        D3DXVec3Normalize(&dir, &dir);


    //  타겟의 위치에서 dir 방향으로 baseDist만큼 떨어진 지점
    float baseDist = 3.7f;
    _vec3 movePos = myPos;

    // 밀어내기
    if (dist < baseDist)
    {
        _vec3 pushDir = myPos - targetPos;
        D3DXVec3Normalize(&pushDir, &pushDir);
        // 뒤로 밀려나는 새로운 위치 계산
        movePos = myPos + pushDir * m_fSpeed * fDeltaTime * 0.66f;
    }

    // 따라가기
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

CGameObject* CMonsterPart::Get_Owner()
{
    CGameObject* pCurrentTarget = m_pTarget;
    CMonsterPart* pNextPart = nullptr;

    // 현재 타겟이 CMonsterPart 타입이라면 계속해서 다음 타겟을 찾아감
    // dynamic_cast를 통해 CMonsterPart로 변환이 성공하는 동안 반복
    while (pNextPart = dynamic_cast<CMonsterPart*>(pCurrentTarget))
    {
        pCurrentTarget = pNextPart->Get_Target();
    }

    // 루프가 끝나면 pCurrentTarget은 더 이상 CMonsterPart가 아닌 최종 타겟(즉, Vellum)을 가리킴
    // 마지막으로 CVellum 타입으로 캐스팅하여 반환
    return pCurrentTarget;
}


void CMonsterPart::Free()
{
    Safe_Release(m_pModel);
    Safe_Release(m_pTransform);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCol);
    Safe_Release(m_pParticle);
    m_pTarget = nullptr;
}
