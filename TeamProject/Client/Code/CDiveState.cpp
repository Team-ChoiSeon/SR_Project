#pragma once
#include "pch.h"
#include "CDiveState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CIdleState.h"

void CDiveState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Dive : Enter\n");
    m_eDPhase = DivePhase::Ready;
    m_fSearch = 0.f;
    m_iCnt = pVellum->Get_PartCnt();
    pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
    if (pVellum->Get_HTransform()->Get_Pos().y < 30.f)
        pVellum->Get_HRigid()->Set_Velocity(_vec3(0.f, 10.f, 0.f));
}

void CDiveState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    CRigidBody* pRigid = pVellum->Get_HRigid();
    CTransform* pTransform = pVellum->Get_HTransform();

    _vec3 dir = pVellum->Get_Target()->Get_Component<CTransform>()->Get_Pos()
        - pTransform->Get_Pos();

    _vec3 diff = dir;
    diff.y = 0.f;
    _float fDist = D3DXVec3Length(&diff);
    D3DXVec3Normalize(&diff, &diff);

    switch (m_eDPhase)
    {
    // force → phase = DiveIn;
    case DivePhase::Ready:
        if (fDist < 5.f)
        {
            pRigid->Stop_Motion();
            m_fSearch = 0.f;
            m_eDPhase = DivePhase::In;
            OutputDebugString(L"Ready->In\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Stop_Motion();
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

     // 도달 체크 → phase = Wait;
    case DivePhase::In:
        if (pTransform->Get_Pos().y < -m_iCnt * 4.f)
        {
            pRigid->Stop_Motion();
            m_eDPhase = DivePhase::Wait;
            OutputDebugString(L"In->Wait\n");
        }
        pRigid->Add_Force({ 0.f,-1.f * 15.f, 0.f });
        break;

     // 시간 경과 → phase = DiveOut;
    case DivePhase::Wait:
        if (fDist < 5.f)
        {
            pRigid->Stop_Motion();
            m_eDPhase = DivePhase::Out;
            m_fSearch = 0.f;
            OutputDebugString(L"Wait->Out\n");
        }
        m_fSearch += fTimeDelta;
        if (m_fSearch > 1.f)
        {
            pRigid->Stop_Motion();
            m_fSearch = 0.f;
        }

        pRigid->Add_Force(diff * 10.f);
        break;

     // 상승 
    case DivePhase::Out:
        if (pTransform->Get_Pos().y > m_iCnt * 4.f)
        {
            pRigid->Stop_Motion();
            OutputDebugString(L"Out\n");

        }
        pRigid->Add_Force({ 0.f,1.f * 15.f, 0.f });
        break;

    }

    if (pTransform->Get_Pos().y > m_iCnt * 4.f && m_eDPhase == DivePhase::Out)
    {

        pVellum->Change_Pattern(new CIdleState());
    }

}

void CDiveState::Exit(CVellum* pVellum)
{
    pVellum->Get_HRigid()->Stop_Motion();
    OutputDebugString(L"Dive : Exit\n");
}
