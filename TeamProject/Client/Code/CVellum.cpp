#pragma once
#include "pch.h"
#include "CVellum.h"
#include "CRigidBody.h"
#include "CCollider.h"


#include "CInputMgr.h"
#include "CSceneMgr.h"

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
    m_vPart.reserve(m_iPartCnt);

    CMonsterPart* pPrev = nullptr;

    for (int i = 0; i < m_iPartCnt; ++i)
    {
        CMonsterPart* pPart = CMonsterPart::Create(m_pGraphicDev);
        if (!pPart)
        {
            MSG_BOX("Vellum 파츠 생성 실패");
            return E_FAIL;
        }

        // 파츠 위치 초기화 (선형 배열 형태)
        _vec3 partPos = { 0.f, - 2.f * i, 0.f };
        pPart->Get_Component<CTransform>()->Set_Pos(partPos);

        // 앞 파츠를 따라가게 연결
        if (pPrev)
            pPart->Set_Target(pPrev);

        m_vPart.push_back(pPart);
        pPrev = pPart;
    }

    m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
   
	return CGameObject::Ready_GameObject();;
}

int CVellum::Update_GameObject(const _float& fTimeDelta)
{
    if (m_vPart.empty())
        return -1;

    // 머리 정보
    CTransform* pHeadTransform = m_vPart[0]->Get_Component<CTransform>();
    CRigidBody* pHeadRigid = m_vPart[0]->Get_Component<CRigidBody>();
    if (!pHeadTransform || !pHeadRigid) return -1;
    _vec3 vPos = pHeadTransform->Get_Pos();

    switch (m_eCuPattern)
    {
    case VPattern::IDLE:
        // 아무것도 하지 않음
        break;

    case VPattern::WANDER:
        Wander(fTimeDelta, pHeadTransform, pHeadRigid);
        break;

    case VPattern::CHASE:
        Chase_Player(fTimeDelta, pHeadTransform, pHeadRigid);
        break;

    case VPattern::AROUND:
        Around_Player(fTimeDelta, pHeadTransform);
        break;
    }

    Update_Pattern(fTimeDelta);

    Key_Input(fTimeDelta);

	CGameObject::Update_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
        pPart->Update_GameObject(fTimeDelta);


	return 0;
}

void CVellum::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
    for (auto* pPart : m_vPart)
        pPart->LateUpdate_GameObject(fTimeDelta);
}

void CVellum::Key_Input(const _float& fTimeDelta)
{
    if (m_vPart.empty())
        return;

    CTransform* pHeadTransform = m_vPart[0]->Get_Component<CTransform>();
    CRigidBody* pRigid = m_vPart[0]->Get_Component<CRigidBody>();
    if (!pHeadTransform || !pRigid)
        return;

    const float speed = 10.0f;
    _vec3 pos = pHeadTransform->Get_Pos();

    if (CInputMgr::Get_Instance()->Key_Down(DIK_UP))
    {
        pRigid->Add_Torque(_vec3(1.f, 0.f, 0.f) * 50.f); //  ???
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
    {
        pRigid->Add_Torque(_vec3(-1.f, 0.f, 0.f) * 50.f); //  ???
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
    {
        pRigid->Add_Torque(_vec3(0.f, 0.f, 1.f) * 50.f); // ???? ???
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
    {
        pRigid->Add_Torque(_vec3(0.f, 0.f, -1.f) * 50.f); // ?????? ???
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

    pHeadTransform->Set_Pos(pos); // 적용
}

void CVellum::Free()
{
    for (auto* pPart : m_vPart)
        Safe_Release(pPart);


    m_vPart.clear();

    CGameObject::Free();

}

void CVellum::Update_Pattern(const _float& fTimeDelta)
{
    m_fPatternTime += fTimeDelta;

    // 패턴 종료 시 → ACTIVE 복구
    if (m_bPattern && m_fPatternTime > m_fSwitchTime)
    {
        if (!m_vPart.empty())
        {
            m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
            m_vPart[0]->Get_Component<CRigidBody>()->Set_Velocity(_vec3(0.f, 0.f, 0.f));
        }
            

        m_bPattern = false;
        m_fPatternTime = 0.f;
        m_fSwitchTime = 3.f + static_cast<float>(rand() % 3);
        m_eCuPattern = VPattern::IDLE;
        return;
    }

    // IDLE 상태일 때만 새로운 패턴 시작
    if (!m_bPattern && m_eCuPattern == VPattern::IDLE && m_fPatternTime > m_fSwitchTime)
    {
        m_eCuPattern = static_cast<VPattern>(rand() % 3); // 0~2 : WANDER, CHASE, AROUND
        m_fPatternTime = 0.f;
        m_fSwitchTime = 5.f + static_cast<float>(rand() % 3); // 다음 전환 대기 시간

        if (!m_vPart.empty())
            m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);

        switch (m_eCuPattern)
        {
        case VPattern::IDLE:
            // 아무것도 하지 않음
            OutputDebugString(L"패턴: IDLE\n");
            break;

        case VPattern::WANDER:
            OutputDebugString(L"패턴: WANDER\n");
            break;

        case VPattern::CHASE:
            OutputDebugString(L"패턴: CHASE\n");
            break;

        case VPattern::AROUND:
            OutputDebugString(L"패턴: AROUND\n");
            break;
        }

        m_bPattern = true;
    }
}

void CVellum::Wander(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid)
{
    //if (m_bPattern && m_fPatternTime == 0.f)
    //{
    //    CGameObject* pPlayer = CSceneMgr::Get_Instance()->Get_Player();
    //    if (pPlayer)
    //    {
    //        _vec3 playerPos = pPlayer->Get_Component<CTransform>()->Get_Pos();
    //        _vec3 offset = { (rand() % 5 - 2.f), 0.f, (rand() % 5 - 2.f) }; // ±2
    //        pTransform->Set_Pos(playerPos + offset);
    //        pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
    //        pRigid->Set_Force({ 0.f, 0.f, 0.f });
    //    }
    //}

    static _float fElapsed = 0.f;
    static _vec3 vDir = { 1.f, 0.f, 0.f };

    fElapsed += fTimeDelta;
    if (fElapsed > 2.f) // 매 2초마다 방향 변경
    {
        float theta = D3DXToRadian(rand() % 360);
        vDir = _vec3(cosf(theta), 0.f, sinf(theta));
        fElapsed = 0.f;
    }

    pRigid->Add_Force(vDir * 5.f);
}

void CVellum::Chase_Player(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid)
{
    CGameObject* pPlayer = CSceneMgr::Get_Instance()->Get_Player();
    if (!pPlayer) return;

    _vec3 myPos = pTransform->Get_Pos();
    _vec3 playerPos = pPlayer->Get_Component<CTransform>()->Get_Pos();
    _vec3 dir = playerPos - myPos;
    D3DXVec3Normalize(&dir, &dir);

    pRigid->Add_Force(dir * 5.f);
}

void CVellum::Around_Player(const _float& fTimeDelta, CTransform* pTransform)
{
    CGameObject* pPlayer = CSceneMgr::Get_Instance()->Get_Player();
    if (!pPlayer) return;

    static float angle = 0.f;

    //if (m_bPattern && m_fPatternTime == 0.f)
    //{
    //    angle = static_cast<float>(rand() % 360);
    //    _vec3 center = pPlayer->Get_Component<CTransform>()->Get_Pos();
    //    float radius = 5.f;
    //    _vec3 startPos = {
    //        center.x + cosf(angle) * radius,
    //        center.y,
    //        center.z + sinf(angle) * radius
    //    };

    //    pTransform->Set_Pos(startPos);
    //}

    angle += fTimeDelta;

    _vec3 center = pPlayer->Get_Component<CTransform>()->Get_Pos();
    float radius = 5.f;
    _vec3 newPos = {
        center.x + cosf(angle) * radius,
        center.y,
        center.z + sinf(angle) * radius
    };

    pTransform->Set_Pos(newPos);
}

REGISTER_GAMEOBJECT(CVellum)