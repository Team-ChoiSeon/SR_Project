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

    // ���� ��ġ ����
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
        else // ��ǥ�� �����ߴٸ�
        {
            m_ePhase = FirePhase::Charge;
            m_fPhaseTime = 0.f;
            pRigid->Stop_Motion();
            // [����] Charge �ܰ��� ���� ��ġ�� ���⼭ �� ���� ����
            m_vBasePos = pTransform->Get_Pos();
        }
        break;
    }

    case FirePhase::Charge:
    {
        // [����] �߻� ������ Charge ���� �� �� ���� ����Ͽ� ���� (�ʿ� ��)
        if (m_fPhaseTime <= fTimeDelta) // �� �ܰ��� ù �������� ��
        {
            _vec3 diff = pTarget->Get_Component<CTransform>()->Get_Pos() - m_vBasePos;
            D3DXVec3Normalize(&m_vDir, &diff);
        }

        pTransform->Set_Look(m_vDir);

        // [����] �Ź� ���� ��ġ�� �ƴ�, ������ m_vChargeBasePos�� �������� ���
        float fMaxlDist = 3.f;
        float fRatio = m_fPhaseTime / m_fChargeTime;
        if (fRatio > 1.f) fRatio = 1.f; // 1�� ���� �ʵ���

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
            // ������: �ڷ� �ִ�� ������ ��ġ
            _vec3 vStartPos = m_vBasePos - (m_vDir * 2.f);
            // ��ǥ��: ���� ��ġ���� �� ������ ���� ��ġ
            _vec3 vTargetPos = m_vBasePos + (m_vDir * fLungeDist);

            _vec3 vCurrentPos;
            float fRatio = m_fPhaseTime / fLungeDuration;
            // Lerp : V1 + s(V2-V1) 
            D3DXVec3Lerp(&vCurrentPos, &vStartPos, &vTargetPos, fRatio);
            pTransform->Set_Pos(vCurrentPos);
        }

        // TODO: ����ü �߻� ����

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
