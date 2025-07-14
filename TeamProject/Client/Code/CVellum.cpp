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
        _vec3 partPos = { 0.f, 20.f - 2.f * i, 0.f };
        pPart->Get_Component<CTransform>()->Set_Pos(partPos);

        // 앞 파츠를 따라가게 연결
        if (pPrev)
            pPart->Set_Target(pPrev);
        
        pPart->Set_Index(i, m_iPartCnt);
        m_vPart.push_back(pPart);
        pPrev = pPart;
    }

    //m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
   
	return CGameObject::Ready_GameObject();;
}

int CVellum::Update_GameObject(const _float& fTimeDelta)
{
    if (m_vPart.empty())
        return -1;

    // 머리 정보
    CTransform* pHeadTransform = m_vPart[0]->Get_Component<CTransform>();
    CRigidBody* pHeadRigid = m_vPart[0]->Get_Component<CRigidBody>();
    
    switch (m_eCurPattern)
    {
    case VPattern::CHASE:
        Chase(fTimeDelta, pHeadTransform, pHeadRigid);
        break;
    case VPattern::DIVE:
        Dive(fTimeDelta, pHeadTransform, pHeadRigid);
        break;
    }

    Update_Pattern(fTimeDelta);

    Key_Input(fTimeDelta);
    for (int i = 1; i < m_iPartCnt; i++)
    {
        /*Snake_Curve(fTimeDelta, m_vPart[i]->Get_Component<CTransform>(), i);*/
    }
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
    for (auto* pPart : m_vPart)
        Safe_Release(pPart);

    m_vPart.clear();

    CGameObject::Free();

}

void CVellum::Update_Pattern(const _float& fTimeDelta)
{
    m_fPatternTime += fTimeDelta;

    if (m_eCurPattern == VPattern::DIVE && m_bPattern)
        return;

    // 패턴 종료 시
    if (m_bPattern && m_fPatternTime > m_fSwitchTime)
    {
        if (!m_vPart.empty())
        {

            m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
            m_vPart[0]->Get_Component<CRigidBody>()->Set_Velocity(_vec3(0.f, 0.f, 0.f));
            m_vPart[0]->Get_Component<CRigidBody>()->Set_AVelocity(_vec3(0.f, 0.f, 0.f));
        }

        m_bPattern = false;
        m_fPatternTime = 0.f;
        m_fSwitchTime = 3.f;    // 다음 패턴까지의 인터벌

        return;
    }

    // 패턴 설정 및 돌입
    if (!m_bPattern && m_fPatternTime >= m_fSwitchTime)
    {
        m_bPattern = true;
        m_fPatternTime = 0.f;

        m_eCurPattern = static_cast<VPattern>(rand() % 2);
        
        // 패턴 재생 시간
        switch (m_eCurPattern)
        {
        case VPattern::CHASE:
            m_fSwitchTime = 3.f;
            break;
        case VPattern::DIVE:
            m_fSwitchTime = FLT_MAX;  // 패턴이 끝날때 까지
            if (m_eDPhase == DivePhase::None)
            {
                m_vPart[0]->Get_Component<CRigidBody>()->Set_Velocity(_vec3(0.f, 10.f, 0.f));
            }
            m_eDPhase = DivePhase::Ready;
            break;
        }

        

        if (!m_vPart.empty())
            m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);

        switch (m_eCurPattern)
        {
        case VPattern::CHASE:  OutputDebugString(L"패턴: CHASE\n"); break;
        case VPattern::DIVE:  OutputDebugString(L"패턴: DIVE\n"); break;
        }
    }
}



void CVellum::Chase(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid)
{
    CGameObject* pPlayer = CSceneMgr::Get_Instance()->Get_Player();
    if (!pPlayer) return;

    _vec3 myPos = pTransform->Get_Pos();
    _vec3 playerPos = pPlayer->Get_Component<CTransform>()->Get_Pos();
    _vec3 dir = playerPos - myPos;

    D3DXVec3Normalize(&dir, &dir);

    pTransform->Set_Look(dir);
    pRigid->Add_Torque(dir * 30.f);
    pRigid->Add_Force(dir * 5.f);
}

void CVellum::Dive(const _float& fTimeDelta, CTransform* pTransform, CRigidBody* pRigid)
{

    CGameObject* pPlayer = CSceneMgr::Get_Instance()->Get_Player();
    if (!pPlayer) return;

    if (!pPlayer || !pTransform || !pRigid)
        return;

    _vec3 myPos = pTransform->Get_Pos();
    _vec3 playerPos = pPlayer->Get_Component<CTransform>()->Get_Pos();
    _vec3 diff = playerPos - myPos;
    diff.y = 0.f;
    _float fDist = D3DXVec3Length(&diff);
    D3DXVec3Normalize(&diff, &diff);

    switch (m_eDPhase)
    {
    // force → phase = DiveIn;
    case DivePhase::Ready:
        if (fDist < 5.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_fSearch = 0.f;
            m_eDPhase = DivePhase::In;
            OutputDebugString(L"Ready->In\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

     // 도달 체크 → phase = Wait;
    case DivePhase::In:
        if (pTransform->Get_Pos().y < -5.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_eDPhase = DivePhase::Wait;
            OutputDebugString(L"In->Wait\n");
        }
        pRigid->Add_Force({ 0.f,-1.f * 15.f, 0.f });
        break;

    // 시간 경과 → phase = DiveOut;
    case DivePhase::Wait:
        if (fDist < 5.f)
        {
            Sleep(300);
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_eDPhase = DivePhase::Out;
            m_fSearch = 0.f;
            OutputDebugString(L"Wait->Out\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

    // 상승 완료 → phase = Ready, m_bPattern = false;
    case DivePhase::Out:
        if (pTransform->Get_Pos().y > 15.f)
        {
            pRigid->Set_Accel({ 0.f,0.f,0.f });
            pRigid->Set_Velocity({ 0.f,0.f,0.f });
            m_eDPhase = DivePhase::None;
            m_vPart[0]->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
            OutputDebugString(L"Out->None\n");

            m_bPattern = false;
            m_fPatternTime = 0.f;
            m_fSwitchTime = 5.f; // 다음 패턴 인터벌
        }
        pRigid->Add_Force({ 0.f,1.f * 15.f, 0.f });
        break;
        
    }
    
}



//void CVellum::Snake_Curve(const _float& fTimeDelta, CTransform* pTransform, int idx)
//{
//    m_fCurveTime += fTimeDelta;
//
//    const _float fAmp = 0.05f; // 진폭
//    const _float fFre = 0.1f;  // 진동수
//    
//    _vec3 vPos = pTransform->Get_Pos();
//    vPos.y += fAmp * sinf(fFre * m_fCurveTime + idx * 0.5f); // 0.5f : 위상 offset
//
//    pTransform->Set_Pos(vPos);
//
//}

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
        pRigid->Add_Torque(_vec3(1.f, 0.f, 0.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
    {
        pRigid->Add_Torque(_vec3(-1.f, 0.f, 0.f) * 50.f); //  
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
    {
        pRigid->Add_Torque(_vec3(0.f, 0.f, 1.f) * 50.f); // 
    }

    if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
    {
        pRigid->Add_Torque(_vec3(0.f, 0.f, -1.f) * 50.f); // 
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

REGISTER_GAMEOBJECT(CVellum)