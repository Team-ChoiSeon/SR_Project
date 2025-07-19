#pragma once
#include "pch.h"
#include "CIntroState.h"
#include "CIdleState.h"

#include "CTransform.h"
#include "CRigidBody.h"

#include "CVellum.h"

#include "CSceneMgr.h"

void CIntroState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Intro : Enter\n");
    m_ePhase = IntroPhase::Orbit;
    m_fRad = 15.f;
    m_fASpeed = 40.f;
    m_fFreq = 3.f;
    m_fAmp = 5.f;
    m_fAngle = 0.f;
    m_vBase = { 0.f, 25.f, 0.f };
}

void CIntroState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    m_fPhaseTime += fTimeDelta;
    CGameObject* pTarget = pVellum->Get_Target();
    CTransform* pTransform = pVellum->Get_HTransform();
    CRigidBody* pRigid = pVellum->Get_HRigid();
    if (!pTransform || !pRigid || !pTarget) return;
    
    _float fRadian;
    _float fX; _float fY; _float fZ;
    float fLungeDuration;
    switch (m_ePhase)
    {
    case IntroPhase::Orbit:
    {
        // 플레이어가 범위에 들어왔는지 체크
        _float X; _float Z;
        if (pTarget)
        {
            CTransform* pTransform = pTarget->Get_Component<CTransform>();
            X = pTransform->Get_Pos().x;
            Z = pTransform->Get_Pos().z;
        }
        if ((X > -60.f && X < 60.f)
            && (Z > -60.f && Z < 60.f))
        {
            // 들어왔을때 중앙으로 이동후 다음패턴
            
            _vec3 vDiff = m_vBase - pTransform->Get_Pos();
            m_vDir = vDiff;
            D3DXVec3Normalize(&m_vDir, &m_vDir);
            pTransform->Set_Look(m_vDir);
            pRigid->Add_Force(m_vDir * 20.f);

            if (D3DXVec3LengthSq(&vDiff) < 1.f)
            {
                m_fPhaseTime = 0.f;
                pRigid->Stop_Motion();
                m_vBase = pTransform->Get_Pos();

                m_ePhase = IntroPhase::Roar;
                
                
            }
        }
        else
        {
            // 회전 움직임
            m_fAngle += m_fASpeed * fTimeDelta;
            fRadian = D3DXToRadian(m_fAngle);
            fX = 0.f + m_fRad * cosf(fRadian) * 1.7f;
            fZ = 60.f + m_fRad * sinf(fRadian) * 1.2f;
            fY = 33.f + m_fAmp * sinf(fRadian * m_fFreq);
            pTransform->Set_Pos({ fX,fY,fZ });
        }
        break;
    }

    case IntroPhase::Charge:
    {
        if (m_fPhaseTime <= fTimeDelta)
        {
            _vec3 diff = pTarget->Get_Component<CTransform>()->Get_Pos() - m_vBase;
            D3DXVec3Normalize(&m_vDir, &diff);
        }

        pTransform->Set_Look(m_vDir);

        _float fMaxlDist = 3.f;
        _float fRatio = m_fPhaseTime / m_fChargeTime;
        if (fRatio > 1.f) fRatio = 1.f;

        _vec3 vPos = m_vBase - (m_vDir * fMaxlDist * fRatio);
        pTransform->Set_Pos(vPos);

        if (m_fPhaseTime >= m_fChargeTime)
        {
            m_ePhase = IntroPhase::Roar;
            m_fPhaseTime = 0.f;
        }
        break;
    }

    case IntroPhase::Roar:
    {
        // Lunge
        float fLungeDuration = 0.25f;
        if (m_fPhaseTime < fLungeDuration)
        {
            float fLungeDist = 2.0f;
            // 시작점: 뒤로 최대로 물러난 위치
            _vec3 vStartPos = m_vBase - (m_vDir * 2.f);
            // 목표점: 기준 위치보다 더 앞으로 나간 위치
            _vec3 vTargetPos = m_vBase + (m_vDir * fLungeDist);

            _vec3 vCurrentPos;
            float fRatio = m_fPhaseTime / fLungeDuration;
            // Lerp : V1 + s(V2-V1) 
            D3DXVec3Lerp(&vCurrentPos, &vStartPos, &vTargetPos, fRatio);
            pTransform->Set_Pos(vCurrentPos);
        }

        if (m_fPhaseTime >= m_fRoarTime)
        {
            m_ePhase = IntroPhase::Orbit;
            m_fPhaseTime = 0.f;
            pVellum->Change_Pattern(new CIdleState());
        }
        break;
    }
    }
}

void CIntroState::Exit(CVellum* pVellum)
{
}
