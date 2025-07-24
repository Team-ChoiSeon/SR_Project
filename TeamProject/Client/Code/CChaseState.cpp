#pragma once
#include "pch.h"
#include "CChaseState.h"
#include "CIdleState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

void CChaseState::Enter(CVellum* pVellum)
{
	OutputDebugString(L"Chase : Enter\n");
	m_fPatternTime = 0.f;
	pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
	m_fSpeed = 20.f;
	m_fPartRatio = 1.f;
}

void CChaseState::Update(const _float fTimeDelta, CVellum* pVellum)
{
	m_fPartRatio = pVellum->Get_PartCnt() / (pVellum->Get_PartCnt() - pVellum->Get_Part().size() +1 );

	_vec3 dir = pVellum->Get_Target()->Get_Component<CTransform>()->Get_Pos()
			  - pVellum->Get_HTransform()->Get_Pos();

	D3DXVec3Normalize(&dir, &dir);
	if (D3DXVec3LengthSq(&dir) > 0.001f)	pVellum->Get_HTransform()->Set_Look(dir);
	

	pVellum->Get_HRigid()->Add_Torque(dir * m_fSpeed * sqrtf(1.f+m_fPartRatio));
	pVellum->Get_HRigid()->Add_Force(dir * 5.f * sqrtf(1.f + m_fPartRatio));

	m_fPatternTime += fTimeDelta;
	if (m_fPatternTime >= m_fSwitchTime / 6.f)
	{
		pVellum->Get_HCol()->Set_ColType(ColliderType::ACTIVE);
	}
	if (m_fPatternTime >= m_fSwitchTime)
	{
		pVellum->Change_Pattern(new CIdleState());
	}
}

void CChaseState::Exit(CVellum* pVellum)
{
	pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
	pVellum->Get_HRigid()->Stop_Motion();
	OutputDebugString(L"Chase : Exit\n");
}
