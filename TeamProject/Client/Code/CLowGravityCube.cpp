#pragma once
#include "pch.h"
#include "CLowGravityCube.h"
#include "CRigidBody.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CTestTile.h"
#include "CMainPlayer.h"

CLowGravityCube::CLowGravityCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CLowGravityCube::CLowGravityCube(const CLowGravityCube& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CLowGravityCube::~CLowGravityCube()
{
}

HRESULT CLowGravityCube::Ready_GameObject()
{
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pRigid->Set_Friction(0.f);
	m_pRigid->Set_Mass(1.f);
	m_pRigid->Set_Bounce(0.7f);
	m_pRigid->Set_OnGround(true);
	m_pRigid->Set_UseGravity(false);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::GROUND);
	m_pCollider->Set_ColType(ColliderType::TRIGGER);
	m_pCollider->Set_BoundType(BoundingType::AABB);

	m_pColTarget = nullptr;
	m_pPreColTarget = nullptr;
	return S_OK;
}

_int CLowGravityCube::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	Detect();
	Push();
	Restoration();
	return _int();
}

void CLowGravityCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CLowGravityCube* CLowGravityCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLowGravityCube* gCube = new CLowGravityCube(pGraphicDev);
	if (FAILED(gCube->Ready_GameObject()))
	{
		Safe_Release(gCube);
		MSG_BOX("Gravity Cube Create Failed");
		return nullptr;
	}
	return gCube;
}

void CLowGravityCube::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRigid);
	Safe_Release(m_pCollider);
	Safe_Release(m_pGraphicDev);
}

void CLowGravityCube::Detect()
{
	CCollider* pOtherCol = m_pCollider->Get_Other();
	if (pOtherCol)
	{
		auto colObj = pOtherCol->m_pOwner;
		if (typeid(*(colObj)) != typeid(CTestTile))
		{
			m_pColTarget = colObj;
		}
	}
}

void CLowGravityCube::Push()
{
	if (m_pColTarget) {
		if (m_bFirstCol) {
			m_fColGravity = m_pColTarget->Get_Component<CRigidBody>()->Get_Gravity();
			m_pColTarget->Get_Component<CRigidBody>()->Set_Gravity(m_fColGravity * 0.3f);
		}
		m_pColTarget->Get_Component<CRigidBody>()->Add_Velocity({ 0.f, 1.f, 0.f });
		m_pPreColTarget = m_pColTarget;
		m_pColTarget = nullptr;
		m_bFirstCol = false;
	}
}

void CLowGravityCube::Restoration()
{
	if (m_pPreColTarget)
	{
		if (m_pPreColTarget->Get_Component<CRigidBody>()->Get_OnGround()) {
			m_pPreColTarget->Get_Component<CRigidBody>()->Set_Gravity(m_fColGravity);
			m_pPreColTarget = nullptr;
			m_bFirstCol = true;
		}
	}
}
