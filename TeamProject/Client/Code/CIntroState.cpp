#pragma once
#include "pch.h"
#include "CIntroState.h"
#include "CIdleState.h"

#include "CTransform.h"
#include "CRigidBody.h"

#include "CVellum.h"
#include "CCinematicCamera.h"
#include "CFirstviewFollowingCamera.h"

#include "CSceneMgr.h"
#include "CInputMgr.h"

void CIntroState::Enter(CVellum* pVellum)
{
    OutputDebugString(L"Intro : Enter\n");
    pVellum->Get_Component<CModel>()->Set_Model(L"Head_Sleep.obj", L"Head_Sleep.mtl");
    //pVellum->Get_Component<CModel>()->Get_Material()->Set_Shader(L"g_UVScale.fx");
    m_ePhase = IntroPhase::Orbit;
    m_fRad = 15.f;
    m_fASpeed = 40.f;
    m_fFreq = 3.f;
    m_fAmp = 5.f;
    m_fAngle = 0.f;
    m_vBase = { 0.f, 15.f, 0.f };
}

void CIntroState::Update(const _float fTimeDelta, CVellum* pVellum)
{
    m_fPhaseTime += fTimeDelta;
    CGameObject* pTarget = pVellum->Get_Target();
    CTransform* pTransform = pVellum->Get_HTransform();
    CRigidBody* pRigid = pVellum->Get_HRigid();
    if (!pTransform || !pRigid || !pTarget) return;

    CTransform* pPlayerTransform = pTarget->Get_Component<CTransform>();
    if (pPlayerTransform)
    {
        _vec3 vLookDir = pTransform->Get_Pos() - pPlayerTransform->Get_Pos();
        D3DXVec3Normalize(&vLookDir, &vLookDir);
        pPlayerTransform->Set_Look(vLookDir);
    }

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
            CSceneMgr::Get_Instance()->Get_Scene()->Get_Layer(LAYER_CAMERA)
                ->Get_GameObject<CCinematicCamera>(L"Cinematic")->Start_Cinematic();
            pTarget->Get_Component<CTransform>()->Set_Pos({0.f,5.f,-59.f});
            pTarget->Get_Component<CRigidBody>()->Set_UseGravity(false);
            // 들어왔을때 중앙으로 이동후 다음패턴
            pVellum->Get_Component<CModel>()->Set_Model(L"Head_Smile.obj", L"Head_Smile.mtl");
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

                m_vPlayerStartPos = pTarget->Get_Component<CTransform>()->Get_Pos();

                m_ePhase = IntroPhase::Charge;
                CSoundMgr::Get_Instance()->Play("Intro");
                
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
            m_vDir = { fX, fY, fZ };
            D3DXVec3Normalize(&m_vDir, &m_vDir);
            pTransform->Set_Look(m_vDir);
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

        CTransform* pPlayerTransform = pTarget->Get_Component<CTransform>();
        if (pPlayerTransform)
        {
            const float fPullDistance = 20.f; // 끌어당길 총 거리
            _vec3 vPullDir = pTransform->Get_Pos() - pPlayerTransform->Get_Pos();
            D3DXVec3Normalize(&vPullDir, &vPullDir);

            // 목표 위치: 시작 위치에서 보스 방향으로 fPullDistance 만큼 이동한 지점
            _vec3 vTargetPos = m_vPlayerStartPos + (vPullDir * fPullDistance);
            _vec3 vCurrentPlayerPos;

            // 차지 시간에 맞춰 플레이어 위치를 선형 보간(Lerp)
            D3DXVec3Lerp(&vCurrentPlayerPos, &m_vPlayerStartPos, &vTargetPos, fRatio);
            pPlayerTransform->Set_Pos(vCurrentPlayerPos);
        }

        if (m_fPhaseTime >= m_fChargeTime)
        {
            m_vPlayerStartPos = pTarget->Get_Component<CTransform>()->Get_Pos();
            m_ePhase = IntroPhase::Roar;
            m_fPhaseTime = 0.f;
        }
        break;
    }

    case IntroPhase::Roar:
    {
        CSceneMgr::Get_Instance()->Get_Scene()->Get_Layer(LAYER_CAMERA)
            ->Get_GameObject<CFirstviewFollowingCamera>(L"MyCamera")->Start_Shake(m_fRoarTime-1.5f, 1.f);

        pVellum->Get_Component<CModel>()->Set_Model(L"Head_Fire.obj", L"Head_Fire.mtl");
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

        CTransform* pPlayerTransform = pTarget->Get_Component<CTransform>();
        if (pPlayerTransform)
        {
            const float fPushDistance = 15.f; // 밀려날 총 거리
            _vec3 vPushDir = m_vDir;

            // 목표 위치: Roar 시작 위치에서 뒤로 fPushDistance 만큼 이동한 지점
            _vec3 vTargetPos = m_vPlayerStartPos + (vPushDir * fPushDistance);
            _vec3 vCurrentPlayerPos;

            // Roar 지속시간에 맞춰 플레이어 위치를 선형 보간(Lerp)
            float fRatio = m_fPhaseTime / m_fRoarTime;
            if (fRatio > 1.f) fRatio = 1.f;

            D3DXVec3Lerp(&vCurrentPlayerPos, &m_vPlayerStartPos, &vTargetPos, fRatio);
            pPlayerTransform->Set_Pos(vCurrentPlayerPos);
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
    CSceneMgr::Get_Instance()->Get_Scene()->Get_Layer(LAYER_CAMERA)
        ->Get_GameObject<CCinematicCamera>(L"Cinematic")->End_Cinematic();
    pVellum->Get_Target()->Get_Component<CRigidBody>()->Set_UseGravity(true);
}
