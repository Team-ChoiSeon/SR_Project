#pragma once
#include "pch.h"
#include "CFireState.h"
#include "CIdleState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

void CFireState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Fire : Enter\n");

    m_ePhase = FirePhase::Prepare;
    m_fPhaseTime = 0.f;

    // 공격 위치 설정
    m_vPos = { 0.f, 10.f, 0.f };
}

void CFireState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    m_fPhaseTime += fTimeDelta; 
    CTransform* pTransform = pVellum->Get_HTransform();
    CRigidBody* pRigid = pVellum->Get_HRigid();
    CGameObject* pTarget = pVellum->Get_Target();
    if (!pTransform || !pRigid || !pTarget) return;

    switch (m_ePhase)
    {
    case FirePhase::Prepare:
    {
        _vec3 diff = m_vPos - pTransform->Get_Pos();
        if (D3DXVec3LengthSq(&diff) > 1.f)
        {
            _vec3 dir = diff;
            D3DXVec3Normalize(&dir, &dir);
            pTransform->Set_Look(dir);
            pRigid->Add_Force(dir * 20.f);
        }
        else // 목표에 도달했다면
        {
            m_ePhase = FirePhase::Charge;
            m_fPhaseTime = 0.f;
            pRigid->Stop_Motion();
            // [수정] Charge 단계의 기준 위치를 여기서 한 번만 저장
            m_vBasePos = pTransform->Get_Pos();
        }
        break;
    }

    case FirePhase::Charge:
    {
        // [수정] 발사 방향은 Charge 시작 시 한 번만 계산하여 고정 (필요 시)
        if (m_fPhaseTime <= fTimeDelta) // 이 단계의 첫 프레임일 때
        {
            _vec3 diff = pTarget->Get_Component<CTransform>()->Get_Pos() - m_vBasePos;
            D3DXVec3Normalize(&m_vDir, &diff);
        }

        pTransform->Set_Look(m_vDir);

        // [수정] 매번 현재 위치가 아닌, 고정된 m_vChargeBasePos를 기준으로 계산
        float fMaxlDist = 3.f;
        float fRatio = m_fPhaseTime / m_fChargeTime;
        if (fRatio > 1.f) fRatio = 1.f; // 1을 넘지 않도록

        _vec3 vPos = m_vBasePos - (m_vDir * fMaxlDist * fRatio);
        pTransform->Set_Pos(vPos);

        if (m_fPhaseTime >= m_fChargeTime)
        {
            m_ePhase = FirePhase::Fire;
            m_fPhaseTime = 0.f;
        }
        break;
    }

    case FirePhase::Fire:
    {
        // Lunge
        float fLungeDuration = 0.25f;
        if (m_fPhaseTime < fLungeDuration)
        {
            float fLungeDist = 2.0f;
            // 시작점: 뒤로 최대로 물러난 위치
            _vec3 vStartPos = m_vBasePos - (m_vDir * 2.f);
            // 목표점: 기준 위치보다 더 앞으로 나간 위치
            _vec3 vTargetPos = m_vBasePos + (m_vDir * fLungeDist);

            _vec3 vCurrentPos;
            float fRatio = m_fPhaseTime / fLungeDuration;
            // Lerp : V1 + s(V2-V1) 
            D3DXVec3Lerp(&vCurrentPos, &vStartPos, &vTargetPos, fRatio);
            pTransform->Set_Pos(vCurrentPos);
        }

        // TODO: 투사체 발사 로직

        if (m_fPhaseTime >= m_fFireTime)
        {
            m_ePhase = FirePhase::Cooldown;
            m_fPhaseTime = 0.f;
        }
        break;
    }

    case FirePhase::Cooldown:
    {
        if (m_fPhaseTime >= m_fCoolTime)
        {
            pVellum->Change_Pattern(new CIdleState());
        }
        break;
    }

    }
}


void CFireState::Exit(CVellum* pVellum)
{
    OutputDebugString(L"Fire : Exit\n");
    pVellum->Get_HRigid()->Stop_Motion();
}
