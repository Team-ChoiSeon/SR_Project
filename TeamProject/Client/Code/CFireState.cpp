#pragma once
#include "pch.h"
#include "CFireState.h"
#include "CIdleState.h"

#include "CSceneMgr.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

#include "CProjectile.h"

void CFireState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Fire : Enter\n");

    m_ePhase = FirePhase::Prepare;
    m_fPhaseTime = 0.f;

    // 공격 위치 설정
    m_vPos = { 0.f, 25.f, 0.f };
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
        else
        {
            m_ePhase = FirePhase::Charge;
            m_fPhaseTime = 0.f;
            pRigid->Stop_Motion();
            
            m_vBase = pTransform->Get_Pos();
        }
        break;
    }

    case FirePhase::Charge:
    {
        
        if (m_fPhaseTime <= fTimeDelta)
        {
            _vec3 diff = pTarget->Get_Component<CTransform>()->Get_Pos() - m_vBase;
            D3DXVec3Normalize(&m_vDir, &diff);
        }

        pTransform->Set_Look(m_vDir);

        float fMaxlDist = 3.f;
        float fRatio = m_fPhaseTime / m_fChargeTime;
        if (fRatio > 1.f) fRatio = 1.f;

        _vec3 vPos = m_vBase - (m_vDir * fMaxlDist * fRatio);
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
            _vec3 vStartPos = m_vBase - (m_vDir * 2.f);
            // 목표점: 기준 위치보다 더 앞으로 나간 위치
            _vec3 vTargetPos = m_vBase + (m_vDir * fLungeDist);

            _vec3 vCurrentPos;
            float fRatio = m_fPhaseTime / fLungeDuration;
            // Lerp : V1 + s(V2-V1) 
            D3DXVec3Lerp(&vCurrentPos, &vStartPos, &vTargetPos, fRatio);
            pTransform->Set_Pos(vCurrentPos);
        }
        else
        {
            if (m_iFireCnt < 7)
            {
                m_fFireDelay += fTimeDelta;
                if (m_fFireDelay > 0.3f)
                {
                    _vec3 vFireDir = Get_TargetDir(pVellum);
                    pTransform->Set_Look(vFireDir);

                    CProjectile* pProjectile = CProjectile::Create(pVellum->Get_Dev());
                    pProjectile->Get_Component<CTransform>()->Set_Pos(pTransform->Get_Pos() + vFireDir * 5.f);
                    pProjectile->Get_Component<CRigidBody>()->Add_Velocity(vFireDir * 20.f);
                    CSceneMgr::Get_Instance()->Get_Scene()->
                        Get_Layer(LAYER_OBJECT)->Add_GameObject(L"projectile" + to_wstring(m_iFireCnt), pProjectile);

                    //OutputDebugString(L"Fire : Shoot~\n");
                    m_fFireDelay = 0.f;
                    m_iFireCnt++;
                }
                
            }
            else
                m_bFire = true;
        }

        if (m_fPhaseTime >= m_fFireTime)
        {
            m_ePhase = FirePhase::Cooldown;
            m_fPhaseTime = 0.f;
            m_iFireCnt = 0;
            m_fFireDelay = 1.f;
            m_bFire = false;
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

_vec3 CFireState::Get_TargetDir(CVellum* pVellum)
{
    CGameObject* pTarget = pVellum->Get_Target();
    CTransform* pMyTransform = pVellum->Get_HTransform();

    if (!pTarget || !pMyTransform)
        return _vec3(0.f, 0.f, 0.f);

    _vec3 vDir;
    _vec3 vTargetPos = pTarget->Get_Component<CTransform>()->Get_Pos();
    _vec3 vMyPos = pMyTransform->Get_Pos();
    _vec3 vDiff = vTargetPos - vMyPos;
    D3DXVec3Normalize(&vDir, &vDiff);

    return vDir;
}
