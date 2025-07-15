#pragma once
#include "pch.h"
#include "CMagneticCube.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPickTarget.h"
#include "CMetalCube.h"
#include "CFactory.h"

CMagneticCube::CMagneticCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CMagneticCube::CMagneticCube(const CMagneticCube& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CMagneticCube::~CMagneticCube()
{
}

HRESULT CMagneticCube::Ready_GameObject()
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
    m_pRigid->Set_Mass(10.f);
    m_pRigid->Set_Bounce(0.f);
    m_pRigid->Set_OnGround(true);
    m_pRigid->Set_UseGravity(false);

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCollider = Get_Component<CCollider>();
    m_pCollider->Set_ColTag(ColliderTag::NONE);
    m_pCollider->Set_ColType(ColliderType::ACTIVE);
    m_pCollider->Set_BoundType(BoundingType::AABB);

    Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
    m_pPick = Get_Component<CPickTarget>();

    CFactory::Save_Prefab(this, "CMagneticCube");
	return S_OK;
}

_int CMagneticCube::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CMagneticCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CMagneticCube::Set_Info()
{
}

CMagneticCube* CMagneticCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	return nullptr;
}

void CMagneticCube::Free()
{
	
}

void CMagneticCube::PickMove()
{
}

void CMagneticCube::DetectMetal()
{
    //m_vecCloseMetal.clear();
    //for (auto& iter : m_vecMetal)
    //{
    //    if (iter->Get_Component<CTransform>()->Get_Pos().x > m_MZone._min.x &&
    //        iter->Get_Component<CTransform>()->Get_Pos().x < m_MZone._max.x &&
    //        iter->Get_Component<CTransform>()->Get_Pos().y > m_MZone._min.y &&
    //        iter->Get_Component<CTransform>()->Get_Pos().y < m_MZone._max.y &&
    //        iter->Get_Component<CTransform>()->Get_Pos().z > m_MZone._min.z &&
    //        iter->Get_Component<CTransform>()->Get_Pos().z < m_MZone._max.z)
    //        m_vecCloseMetal.push_back(iter);
    //}

    //for (auto& iter : m_vecCloseMetal)
    //{
    //    iter->GluetoMagnetic(this);
    //}
}

REGISTER_GAMEOBJECT(CMagneticCube)