#pragma once
#include "pch.h"
#include "CIntroState.h"
#include "CIdleState.h"

#include "CTransform.h"

#include "CVellum.h"

void CIntroState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Intro : Enter\n");
    m_ePhase = IntroPhase::Orbit;
    m_fRad = 15.f;
    m_fASpeed = 40.f;
    m_fFreq = 3.f;
    m_fAmp = 3.5f;
    m_fAngle = 0.f;
    m_fRoarTime = 0.f;
}

void CIntroState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    CGameObject* pTarget = pVellum->Get_Target();
    CTransform* pTransform = pVellum->Get_HTransform();

    _vec3 vDiff;
    _float fRadian;
    _float fX; _float fY; _float fZ;
    float fLungeDuration;
    switch (m_ePhase)
    {
    case IntroPhase::Orbit:
        m_fAngle += m_fASpeed * fTimeDelta;
        fRadian = D3DXToRadian(m_fAngle);
        fX = pTransform->Get_Pos().x + m_fRad * cosf(fRadian);
        fZ = pTransform->Get_Pos().z + m_fRad * sinf(fRadian);
        fY = pTransform->Get_Pos().y + m_fAmp * sinf(fRadian * m_fFreq);

        pTransform->Set_Pos({ fX,fY,fZ });

        if (m_fAngle <= 360.f)
        {
            m_ePhase = IntroPhase::Roar;
        }
        break;

    case IntroPhase::Roar:
        m_fPhaseTime += fTimeDelta;

        if (m_fPhaseTime < 2.5f)
        {
            m_vBasePos = { 0.f, 10.f, 0.f };
            vDiff = pTarget->Get_Component<CTransform>()->Get_Pos() - m_vBasePos;
            D3DXVec3Normalize(&m_vDir, &vDiff);
            pTransform->Set_Look(m_vDir);

            float fMaxlDist = 3.f;
            float fRatio = m_fPhaseTime / 2.5f;
            if (fRatio > 1.f) fRatio = 1.f;

            _vec3 vPos = m_vBasePos - (m_vDir * fMaxlDist * fRatio);
            pTransform->Set_Pos(vPos * 0.5f);
        }

        fLungeDuration = 0.25f;
        if (m_fRoarTime < fLungeDuration && m_fPhaseTime >= 2.5f)
        {
            float fLungeDist = 2.0f;
            // 시작점: 뒤로 최대로 물러난 위치
            _vec3 vStartPos = m_vBasePos - (m_vDir * 2.f);
            // 목표점: 기준 위치보다 더 앞으로 나간 위치
            _vec3 vTargetPos = m_vBasePos + (m_vDir * fLungeDist);

            _vec3 vCurrentPos;
            float fRatio = m_fRoarTime / fLungeDuration;
            // Lerp : V1 + s(V2-V1) 
            D3DXVec3Lerp(&vCurrentPos, &vStartPos, &vTargetPos, fRatio);
            pTransform->Set_Pos(vCurrentPos * 0.5f);
            m_fRoarTime += fTimeDelta;
        }
        else if(m_fRoarTime >= fLungeDuration && m_fPhaseTime >= 2.5f)
        {
            m_ePhase = IntroPhase::End;
            pVellum->Change_Pattern(new CIdleState());
        }
        break;
    case IntroPhase::End:
        break;
    }
}

void CIntroState::Exit(CVellum* pVellum)
{
}
