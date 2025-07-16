#pragma once
#include "pch.h"
#include "CMagneticCube.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPickTarget.h"
#include "CMetalCube.h"
#include "CFactory.h"
#include "CMainPlayer.h"
#include "CPickingMgr.h"
#include "CCameraMgr.h"

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
    m_pRigid->Set_Mass(1.f);
    m_pRigid->Set_Bounce(0.f);
    m_pRigid->Set_OnGround(false);
    m_pRigid->Set_UseGravity(true);

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
    CGameObject::Update_GameObject(fTimeDelta);
    PickMove();
    if(m_pRigid->Get_OnGround())
        m_pCollider->Set_ColType(ColliderType::PASSIVE);
	return _int();
}

void CMagneticCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMagneticCube::Set_Info()
{
}

CMagneticCube* CMagneticCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMagneticCube* mCube = new CMagneticCube(pGraphicDev);
    if (FAILED(mCube->Ready_GameObject()))
    {
        Safe_Release(mCube);
        MSG_BOX("MCube Create Failed");
        return nullptr;
    }
    return mCube;
}

void CMagneticCube::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pModel);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCollider);
    Safe_Release(m_pPick);
    Safe_Release(m_pGraphicDev);
}

void CMagneticCube::PickMove()
{
    if (m_bCurGrab)
    {
        m_pCollider->Set_ColType(ColliderType::PASSIVE);
        m_pRigid->Set_UseGravity(false);
        m_pRigid->Set_OnGround(true);
        m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
        m_pTransform->Set_Pos(m_pTransform->Get_Pos() +m_vCursorDelta);
    }
    else {
        m_pCollider->Set_ColType(ColliderType::ACTIVE);
        m_pRigid->Set_UseGravity(true);
        m_pRigid->Set_OnGround(false);
    }
}

REGISTER_GAMEOBJECT(CMagneticCube)