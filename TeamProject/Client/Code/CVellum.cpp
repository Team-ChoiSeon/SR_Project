#pragma once
#include "pch.h"
#include "CVellum.h"

#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CParticle.h"

#include "CInputMgr.h"
#include "CSceneMgr.h"

#include "CIdleState.h"
#include "CIntroState.h"

#include "CFactory.h"



CVellum::CVellum(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{

}

CVellum::~CVellum()
{
}

CVellum* CVellum::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CVellum* pVellum = new CVellum(pGraphicDev);

	if (FAILED(pVellum->Ready_GameObject()))
	{
		Safe_Release(pVellum);
		MSG_BOX("Vellum Create Failed");
		return nullptr;
	}

	return pVellum;
}

HRESULT CVellum::Ready_GameObject()
{
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();

    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCol = Get_Component<CCollider>();

    Add_Component<CParticle>(ID_DYNAMIC, m_pGraphicDev);
    m_pParticle = Get_Component<CParticle>();
    m_pParticle->Set_Texture(L"blackSmoke00.png");
    m_pParticle->Set_Type(PARTICLE_MOVE_TYPE::BREATH);
    m_pParticle->Set_MaxParticle(200);
    m_pParticle->Set_SpawnInterval(0.1f);


    m_pTransform->Set_Pos({ 0.f, 33.f, 60.f });  
    m_pTransform->Set_Scale({ 3.f, 3.f, 3.f });

    m_pRigid->Set_OnGround(false);
    m_pRigid->Set_UseGravity(false);
    m_pRigid->Set_Mass(1.f);
    m_pRigid->Set_Friction(1.f);
    m_pRigid->Set_Bounce(0.f);

    m_pCol->Set_ColTag(ColliderTag::MONSTER);
    m_pCol->Set_ColType(ColliderType::PASSIVE);
    m_pCol->Set_BoundType(BoundingType::OBB);


    m_vPart.reserve(m_iPartCnt);
    CGameObject* pTarget = this;

    for (int i = 0; i < m_iPartCnt; ++i)
    {
        CMonsterPart* pPart = CMonsterPart::Create(m_pGraphicDev);
        if (!pPart) return E_FAIL;

        // 파츠 위치 초기화 (선형 배열 형태)
        _vec3 headPos = m_pTransform->Get_Info(INFO_POS);
        _vec3 partPos = headPos - _vec3(0.f, 4.f * (i + 1), 0.f);
        pPart->Get_Component<CTransform>()->Set_Pos(partPos);

        pPart->Set_Target(pTarget);
        pPart->Set_Index(i, m_iPartCnt);
        m_vPart.push_back(pPart);
        pTarget = pPart;
    }

    m_pTarget = CSceneMgr::Get_Instance()->Get_Player();

    m_pState = new CIntroState();
    m_pState->Enter(this);

    CFactory::Save_Prefab(this, "CVellum");
	return CGameObject::Ready_GameObject();;
}

int CVellum::Update_GameObject(const _float& fTimeDelta)
{
    if (m_vPart.empty()) return -1;


    CMonsterPart* pPartToDestroy = nullptr;

    for (CMonsterPart* pPart : m_vPart)
    {
        CCollider* pCollider = pPart->Get_Component<CCollider>();
        if (pCollider && pCollider->Get_ColState() == Engine::ColliderState::ENTER)
        {
            CCollider* pOther = pCollider->Get_Other();
            if (pOther && pOther->Get_ColTag() == Engine::ColliderTag::ATTACK)
            {
                pPartToDestroy = pPart;
                break;
            }
        }
    }


    if (pPartToDestroy)
    {
        Organize_Chain(pPartToDestroy);
    }


    if (m_pState)
        m_pState->Update(fTimeDelta, this);

    Key_Input(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
    {
        pPart->Update_GameObject(fTimeDelta);
    }

	return 0;
}

void CVellum::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
        pPart->LateUpdate_GameObject(fTimeDelta);
}


void CVellum::Free()
{
    Safe_Delete(m_pState);

    m_pTransform = nullptr;
    m_pRigid = nullptr;
    m_pCol = nullptr;
    m_pParticle = nullptr;

    for (auto* pPart : m_vPart)
        Safe_Release(pPart);
    m_vPart.clear();

    CGameObject::Free();
}


void CVellum::Change_Pattern(IVellumState* pState)
{
    if (m_pState) 
    {
        m_pState->Exit(this);
        Safe_Delete(m_pState);
    }

    m_pState = pState;
    if (m_pState)  m_pState->Enter(this);
}



void CVellum::Organize_Chain(CMonsterPart* pPart)
{
    auto iter = find(m_vPart.begin(), m_vPart.end(), pPart);
    if (iter == m_vPart.end())
        return;

    // 앞/뒤 파츠 찾기 
    size_t index = distance(m_vPart.begin(), iter);
    CGameObject* pPrecedingPart = nullptr;
    if (index == 0)
    {
        pPrecedingPart = this;
    }
    else
    {
        pPrecedingPart = m_vPart[index - 1];
    }

    CMonsterPart* pSucceedingPart = (index + 1 < m_vPart.size()) ? m_vPart[index + 1] : nullptr;

    if (pSucceedingPart)
    {
        pSucceedingPart->Set_Target(pPrecedingPart);
    }

    Safe_Release(*iter);
    m_vPart.erase(iter);

    for (size_t i = 0; i < m_vPart.size(); ++i)
        m_vPart[i]->Set_Index(i, m_vPart.size());
}

void CVellum::Key_Input(const _float& fTimeDelta)
{
    if (m_vPart.empty())
        return;

    if (!m_pTransform || !m_pRigid)
        return;

    const float speed = 10.0f;
    _vec3 pos = m_pTransform->Get_Pos();

    if (CInputMgr::Get_Instance()->Key_Down(DIK_UP))
    {
        m_pRigid->Add_Torque(_vec3(1.f, 0.f, 0.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
    {
        m_pRigid->Add_Torque(_vec3(-1.f, 0.f, 0.f) * 50.f); //  
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
    {
        m_pRigid->Add_Torque(_vec3(0.f, 0.f, 1.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
    {
        m_pRigid->Add_Torque(_vec3(0.f, 0.f, -1.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD8)) // +Y
        pos.y += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD2)) // -Y
        pos.y -= speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD6)) // +X
        pos.x += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD4)) // -X
        pos.x -= speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD9)) // +Z
        pos.z += speed * fTimeDelta;
    if (CInputMgr::Get_Instance()->Key_Down(DIK_NUMPAD7)) // -Z
        pos.z -= speed * fTimeDelta;

    // 임시 삭제 코든
    if (CInputMgr::Get_Instance()->Key_Away(DIK_1))
    {
        if (!m_vPart.empty() && m_vPart.size() > 1)
        {
            CMonsterPart* pTargetPart = m_vPart[1]; 
            Organize_Chain(pTargetPart);
        }
    }
 

    m_pTransform->Set_Pos(pos); // ?곸슜
}

REGISTER_GAMEOBJECT(CVellum)