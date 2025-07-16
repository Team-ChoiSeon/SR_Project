#pragma once
#include "pch.h"
#include "CProjectile.h"

#include "CModel.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"

CProjectile::CProjectile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CProjectile::CProjectile(const CProjectile& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CProjectile::~CProjectile()
{
}

HRESULT CProjectile::Ready_GameObject()
{
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Set_Scale({ 0.33f, 0.33f, 0.33f });

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_OnGround(false);
	m_pRigid->Set_UseGravity(false);
	m_pRigid->Set_Bounce(0);
	m_pRigid->Set_Friction(1);
	m_pRigid->Set_Mass(1.f);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCol = Get_Component<CCollider>();
	m_pCol->Set_ColTag(ColliderTag::NONE);
	m_pCol->Set_ColType(ColliderType::PASSIVE);
	m_pCol->Set_BoundType(BoundingType::OBB);

	return CGameObject::Ready_GameObject();
}

_int CProjectile::Update_GameObject(const _float& fTimeDelta)
{
	m_fLifeTime -= fTimeDelta;
	if (m_fLifeTime <= 0.f || m_pCol->Get_ColState()==ColliderState::ENTER)
	{
		return 1;
	}
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CProjectile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CProjectile* CProjectile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	auto pPro = new CProjectile(pGraphicDev);
	if (FAILED(pPro->Ready_GameObject())) {
		Safe_Release(pPro);
		return nullptr;
	}
	return pPro;
}

void CProjectile::Free()
{
	Safe_Release(m_pModel);
	Safe_Release(m_pTransform);
	Safe_Release(m_pRigid);
	Safe_Release(m_pCol);
	CGameObject::Free();
}
