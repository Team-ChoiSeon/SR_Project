#pragma once
#include "pch.h"
#include "CDeadState.h"

#include "CVellum.h"

#include "CModel.h"
#include "CCollider.h"

void CDeadState::Enter(CVellum* pVellum)
{
	// sound
	m_fDeadTime = 0.f;
    m_fDuration = 10.f;
	// color
}

void CDeadState::Update(const _float fTimeDelta, CVellum* pVellum)
{
	m_fDeadTime += fTimeDelta;
	
    if (m_fDeadTime >= m_fDuration)
    {
        Exit(pVellum);
        return;
    }
    
    CModel* pModel = pVellum->Get_Component<CModel>();
    if (!pModel) return;

    if (m_fDeadTime >= m_fDuration - 1.f)
    {
        pModel->Set_Alpha(1.f);
        // explosion...
    }
    else 
    {
        const _float fSpeed = 10.f;
        const _float fBlinkingDuration = m_fDuration - 1.f;
        _float alpha = 1.f - (m_fDeadTime / fBlinkingDuration);
        alpha *= (0.5f + 0.5f * sin(m_fDeadTime * fSpeed * D3DX_PI));
        pModel->Set_Alpha(alpha < 0.f ? 0.f : alpha);
    }
}

void CDeadState::Exit(CVellum* pVellum)
{
    pVellum->Get_Component<CCollider>()->Set_Active(false);
    pVellum->Get_Component<CModel>()->Set_Active(false);
	pVellum->Set_Dead(true);
}
