#pragma once
#include "pch.h"
#include "CTestTile.h"

#include "CFactory.h"

CTestTile::CTestTile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CTestTile::~CTestTile()
{

}

HRESULT CTestTile::Ready_GameObject()
{
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Pos({ 1.f, -2.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_UseGravity(false);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::GROUND);
	m_pCollider->Set_ColType(ColliderType::PASSIVE);


	
	Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Component<CRigidBody>()->Set_UseGravity(false);


	CFactory::Save_Prefab(this, "CTestTile");

	return S_OK;
}

int CTestTile::Update_GameObject(const _float& fTimeDelta)
{
	float blinkSpeed = 10.f; // 깜빡이는 속도
	float alpha = 0.f;

	switch (m_eState)
	{
	case ETileState::IDLE:
		// 아무것도 하지 않음
		break;
	case ETileState::DESTROYING:
		m_fETimer += fTimeDelta;
		// 파괴 전 깜빡임 효과
		alpha = 1.f - (m_fETimer / m_fEDuration);
		alpha *= (0.5f + 0.5f * sin(m_fETimer * blinkSpeed * D3DX_PI)); // 깜빡임
		if (m_pModel) m_pModel->Set_Alpha(alpha < 0.f ? 0.f : alpha);
		// 효과 시간이 끝나면 완전 파괴 상태로 전환
		if (m_fETimer >= m_fEDuration)
		{
			m_eState = ETileState::DESTROYED;
			m_fRTimer = 0.f;
			if (m_pModel)    m_pModel->Set_Active(false);
			if (m_pCollider) m_pCollider->Set_Active(false);
		}
		break;
	case ETileState::DESTROYED:
		m_fRTimer += fTimeDelta;
		// 복구 대기 시간이 끝나면 복구 시작
		if (m_fRTimer >= m_fRDuration)
		{
			m_eState = ETileState::RESTORING;
			m_fETimer = 0.f;
			if (m_pModel)    m_pModel->Set_Active(true);
			if (m_pCollider) m_pCollider->Set_Active(true);
		}
		break;
	case ETileState::RESTORING:
		m_fETimer += fTimeDelta;

		// 복구 시 깜빡임 효과
		alpha = m_fETimer / m_fEDuration; // 점점 진해지며
		alpha *= (0.5f + 0.5f * sin(m_fETimer * blinkSpeed * D3DX_PI)); // 깜빡임
		if (m_pModel) m_pModel->Set_Alpha(alpha > 1.f ? 1.f : alpha);
		// 효과 시간이 끝나면 평상시 상태로 전환
		if (m_fETimer >= m_fEDuration)
		{
			m_eState = ETileState::IDLE;
			if (m_pModel) m_pModel->Set_Alpha(1.f); // 알파 값을 1로 완전히 복구
		}
		break;
	}



	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	return 0;
}

void CTestTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component(fTimeDelta);
}

CTestTile* CTestTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestTile* pTile = new CTestTile(pGraphicDev);

	if (FAILED(pTile->Ready_GameObject()))
	{
		Safe_Release(pTile);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pTile;
}
void CTestTile::Free()
{
	Safe_Release(m_pRigid);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
}

void CTestTile::Set_Destroy(bool bDestroy)
{
	if (bDestroy && m_eState == ETileState::IDLE)
	{
		m_eState = ETileState::DESTROYING;
		m_fETimer = 0.f;
	}
}



REGISTER_GAMEOBJECT(CTestTile)