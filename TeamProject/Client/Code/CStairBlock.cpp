#include "pch.h"
#include "Engine_Define.h"
#include "CStairBlock.h"
#include "CTransform.h"
#include "CSceneMgr.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CFactory.h"

CStairBlock::CStairBlock(LPDIRECT3DDEVICE9 pDevice)
    : CBlock(pDevice),m_fDistance(0.f), m_fnowHeight(0.f), m_bFindTarget(false), m_pTarget(nullptr)
{

}

CStairBlock::~CStairBlock()
{
}

CStairBlock* CStairBlock::Create(LPDIRECT3DDEVICE9 pDevice)
{
    CStairBlock* instance = new CStairBlock(pDevice);

    if (FAILED(instance->Ready_GameObject())) {
        Safe_Release(instance);
        instance = nullptr;
    }

    return instance;
}

HRESULT CStairBlock::Ready_GameObject()
{
   
    m_pRigid->Set_UseGravity(false);
    m_pRigid->Set_OnGround(true);
    m_pCollider->Set_ColTag(ColliderTag::GROUND);
    m_pCollider->Set_ColType(ColliderType::PASSIVE);
    m_pCollider->Set_BoundType(BoundingType::AABB);

    CFactory::Save_Prefab(this, "CStairBlock");
    return S_OK;
}
_int CStairBlock::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    if (!m_pTarget) {
        m_pTarget = CSceneMgr::Get_Instance()->Get_Player();
        return 0;
    }

    if (m_bFindTarget) return 0;

    CTransform* targetTrans = m_pTarget->Get_Component<CTransform>();
    _vec3 targetDistance = m_pTransform->Get_Pos() - targetTrans->Get_Pos();

    constexpr float kTriggerX = 5.f;
    constexpr float kTriggerZ = 5.f;
    constexpr float kTriggerY = 0.f;

    if (targetDistance.y < kTriggerY &&
        fabs(targetDistance.x) < kTriggerX &&
        fabs(targetDistance.z) < kTriggerZ) {
        m_bFindTarget = true;
    }

    return 0;
}

void CStairBlock::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);

    if (!m_bFindTarget) return;

    if (m_fnowHeight < m_fDistance) {
        _float moveAmount = fTimeDelta;
        if (m_fnowHeight + moveAmount > m_fDistance)
            moveAmount = m_fDistance - m_fnowHeight;

        m_fnowHeight += moveAmount;

        _vec3 pos = m_pTransform->Get_Pos();
        pos.y += moveAmount;
        m_pTransform->Set_Pos(pos);
    }
}

void CStairBlock::Free()
{
    CGameObject::Free();
}

REGISTER_GAMEOBJECT(CStairBlock)