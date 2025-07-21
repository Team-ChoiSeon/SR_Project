#pragma once
#include "pch.h"
#include "CZoneSensor.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CMainPlayer.h"
#include "CFactory.h"

CZoneSensor::CZoneSensor(LPDIRECT3DDEVICE9 pGraphicDev)
	:CSensor(pGraphicDev)
{
}

CZoneSensor::CZoneSensor(const CZoneSensor& rhs)
	:CSensor(rhs.m_pGraphicDev)
{
}

CZoneSensor::~CZoneSensor()
{
}

HRESULT CZoneSensor::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();
    m_pTransform->Ready_Component();
    m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
    m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
    m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();
    m_pRigid->Set_Friction(0.f);
    m_pRigid->Set_Mass(1.f);
    m_pRigid->Set_Bounce(0.f);
    m_pRigid->Set_OnGround(true);
    m_pRigid->Set_UseGravity(false);

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCollider = Get_Component<CCollider>();
    m_pCollider->Set_ColTag(ColliderTag::GROUND);
    m_pCollider->Set_ColType(ColliderType::TRIGGER);
    m_pCollider->Set_BoundType(BoundingType::AABB);

    m_bSensorOn = false;

	CFactory::Save_Prefab(this, "CZoneSensor");
	return S_OK;
}

_int CZoneSensor::Update_GameObject(const _float& fTimeDelta)
{
    m_bSensorOn = Detect();
	CGameObject::Update_GameObject(fTimeDelta);
	return _int();
}

void CZoneSensor::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CZoneSensor* CZoneSensor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CZoneSensor* zSensor = new CZoneSensor(pGraphicDev);
    if (FAILED(zSensor->Ready_GameObject()))
    {
        Safe_Release(zSensor);
        MSG_BOX("ZoneSensor Create Failed");
        return nullptr;
    }
    return zSensor;
}

void CZoneSensor::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCollider);
}

_bool CZoneSensor::Detect()
{
    if (auto othercol = m_pCollider->Get_Other())
    {
        if (typeid(*(othercol->m_pOwner)) == typeid(CMainPlayer))
            return true;
    }
    return false;
}

_bool CZoneSensor::DetectObj(CGameObject* gameobj)
{
    if (auto othercol = m_pCollider->Get_Other())
    {
        if (typeid(*(othercol->m_pOwner)) == typeid(gameobj))
            return true;
    }
	return false;
}


REGISTER_GAMEOBJECT(CZoneSensor)