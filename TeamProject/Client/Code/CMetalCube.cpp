#pragma once
#include "pch.h"
#include "CMetalCube.h"
#include "CMagneticCube.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"

CMetalCube::CMetalCube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CCube(pGraphicDev)
{
}

CMetalCube::CMetalCube(const CMetalCube& rhs)
    : CCube(rhs.m_pGraphicDev)
{
}

CMetalCube::~CMetalCube()
{
}

HRESULT CMetalCube::Ready_GameObject()
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
    m_pCollider->Set_ColTag(ColliderTag::GROUND);
    m_pCollider->Set_ColType(ColliderType::PASSIVE);

    return S_OK;
}

_int CMetalCube::Update_GameObject(const _float& fTimeDelta)
{
    return _int();
}

void CMetalCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CMetalCube::Set_Info()
{
}

CMetalCube* CMetalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    return nullptr;
}

void CMetalCube::Free()
{
}

void CMetalCube::MovetoMagnetic(CMagneticCube* parentMagnet)
{
    m_pParentMagnet = parentMagnet;
    
    _vec3 ParentPos = m_pParentMagnet->Get_Component<CTransform>()->Get_Pos();
    _vec3 ParentScale = m_pParentMagnet->Get_Component<CTransform>()->Get_Scale();
    _vec3 gap = m_pTransform->Get_Pos() - ParentPos;
    _float fgap = D3DXVec3Length(&gap);

    
}
