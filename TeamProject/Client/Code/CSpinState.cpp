#pragma once
#include "pch.h"
#include "CSpinState.h"
#include "CIdleState.h"

#include "CVellum.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

#include "CProjectile.h"

#include "CSceneMgr.h"

void CSpinState::Enter(CVellum* pVellum)
{
	OutputDebugString(L"Spin : Enter\n");
	pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
	if (pVellum->Get_HTransform()->Get_Pos().y < 10.f)
	{
		pVellum->Get_HRigid()->Set_Velocity(_vec3(0.f, 5.f, 0.f));
	}
	m_fPatternTime = 0.f;
	m_fSwitchTime = 3.f;
	m_fSpeed = 30.f;
	m_fFireDelay = 0.f;
}

void CSpinState::Update(const _float fTimeDelta, CVellum* pVellum)
{
	vector<CMonsterPart*> pPart = pVellum->Get_Part();
	for (auto part : pPart)
	{
		part->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
		part->Get_Component<CRigidBody>()->Add_Torque({ 0.f, 1.f * m_fSpeed, 0.f });
	}
	
	const float fInterval = 1.f;

	m_fFireDelay += fTimeDelta; 
	if (m_fFireDelay >= fInterval)
	{
		m_fFireDelay = 0.f; 

		_vec3 vVellumPos = pVellum->Get_HTransform()->Get_Pos();

		auto pPart = pVellum->Get_Part();
		for (size_t i = 0; i < pPart.size(); ++i)
		{
			if (i % 2 != 0) continue;

			CTransform* pPartTransform = pPart[i]->Get_Component<CTransform>();
			_vec3 vPartPos = pPartTransform->Get_Pos();
			_vec3 vFireDir = vPartPos - vVellumPos;
			D3DXVec3Normalize(&vFireDir, &vFireDir);

			
			CProjectile* pProjectile = CProjectile::Create(pVellum->Get_Dev());

			
			pProjectile->Get_Component<CTransform>()->Set_Pos(vPartPos + vFireDir * 1.f);
			pProjectile->Get_Component<CRigidBody>()->Add_Velocity(vFireDir * m_fSpeed);

		
			static int iProjectileCnt = 0;
			CSceneMgr::Get_Instance()->Get_Scene()->
				Get_Layer(LAYER_OBJECT)->Add_GameObject(L"SpinProjectile_" + to_wstring(iProjectileCnt++), pProjectile);
		}
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
	pVellum->Get_HCol()->Set_ColType(ColliderType::PASSIVE);
}
