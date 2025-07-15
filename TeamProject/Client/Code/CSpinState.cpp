#pragma once
#include "pch.h"
#include "CSpinState.h"
#include "CIdleState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

void CSpinState::Enter(CVellum* pVellum)
{
	OutputDebugString(L"Spin : Enter\n");
	if (pVellum->Get_HTransform()->Get_Pos().y < 20.f)
	{
		pVellum->Get_HRigid()->Set_Velocity(_vec3(0.f, 10.f, 0.f));
	}
	m_fPatternTime = 0.f;
	m_fSwitchTime = 3.f;
	m_fSpeed = 30.f;
}

void CSpinState::Update(const _float fTimeDelta, CVellum* pVellum)
{
	vector<CMonsterPart*> pPart = pVellum->Get_Part();
	for (auto part : pPart)
	{
		part->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
		part->Get_Component<CRigidBody>()->Add_Torque({ 0.f, 1.f * m_fSpeed, 0.f });
	}
	
	m_fPatternTime += fTimeDelta;
	if (m_fPatternTime >= m_fSwitchTime)
	{
		pVellum->Change_Pattern(new CIdleState());
	}

}

void CSpinState::Exit(CVellum* pVellum)
{
	OutputDebugString(L"Spin : Exit\n");
	vector<CMonsterPart*> pPart = pVellum->Get_Part();
	for (auto part : pPart)
	{
		part->Get_Component<CRigidBody>()->Stop_AMotion();
		part->Get_Component<CRigidBody>()->Stop_Motion();
		part->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	}
}
