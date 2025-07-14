#include "CMonsterPart.h"

#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h" 
#include "CFactory.h"
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

    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCol = Get_Component<CCollider>();
    m_pCol->Set_ColTag(ColliderTag::NONE);
    m_pCol->Set_ColType(ColliderType::PASSIVE);
    m_pCol->Set_BoundType(BoundingType::OBB);

    m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

    m_pRigid->Set_OnGround(false);
    m_pRigid->Set_UseGravity(false);
    m_pRigid->Set_Mass(1.f);
    m_pRigid->Set_Friction(1.f);
    m_pRigid->Set_Bounce(0.f);

    CFactory::Save_Prefab(this, "CMonsterPart");

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

    // 1. 방향과 거리 계산
    _vec3 dir = targetPos - myPos;
    float dist = D3DXVec3Length(&dir);
    if (dist < 0.001f) return; // 너무 가까우면 처리 X

    // 2. 방향 벡터 정규화
    D3DXVec3Normalize(&dir, &dir);

    // 3. 이동 방향 처리 (Set_Look + Set_Right + Set_Up)
    m_pTransform->Set_Look(dir);

    _vec3 vUp = { 0.f, 1.f, 0.f };
    _vec3 vRight;
    D3DXVec3Cross(&vRight, &vUp, &dir);
    D3DXVec3Normalize(&vRight, &vRight);

    _vec3 vNewUp;
    D3DXVec3Cross(&vNewUp, &dir, &vRight);

    m_pTransform->Set_Right(vRight);
    m_pTransform->Set_Up(vNewUp);

    // 충돌 중엔 위치보정 중단 (강한 당김 방지)
    /*if (m_pCol->Get_ColState() == ColliderState::STAY)
        return; */

    // 4. 스프링 물리 기반 위치 이동 (Hooke's Law)
    // F = -k * x - b * v
    
    // 거리가 멀어질수록 더 빠르게 이동
    /*
    * 머리 근처 파츠가 덜렁거리면 > damping 증가
    * 꼬리 파츠가 너무 뒤쳐지면 > stiffness 증가 
    */

    _float ratio = (_float)m_iIdx / max(1, m_iMax - 1);
    _float baseDist = 2.f; // 기준 거리
    const _float baseStiffness = 60.f;
    const _float baseDamping = 6.f;
    
    _vec3 targetSpringPos = targetPos - dir * baseDist;
    _float stretchFactor = max(1.f, dist / baseDist); // 최소 1.0 이상
    _float stiffness = baseStiffness * stretchFactor;  // 멀어질수록 더 강하게
    _float damping = baseDamping * sqrtf(stretchFactor);  // 감쇠는 급격히 커지지 않도록 루트 적용

    _vec3 springVec = targetSpringPos - myPos;
    _vec3 velocity = m_pRigid->Get_Velocity();
    _vec3 force = springVec * stiffness - velocity * damping;


    // 외력으로 전달
    m_pRigid->Add_Force(force);
}


void CMonsterPart::Free()
{
    Safe_Release(m_pModel);
    Safe_Release(m_pTransform);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCol);
    m_pTarget = nullptr;
}

REGISTER_GAMEOBJECT(CMonsterPart)