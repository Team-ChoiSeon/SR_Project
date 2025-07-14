#pragma once
#include "pch.h"
#include "CMagneticCube.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPickTarget.h"
#include "CMetalCube.h"


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
    m_pCollider->Set_ColType(ColliderType::PASSIVE);
    m_pCollider->Set_BoundType(BoundingType::OBB);


    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pDetectCollider = Get_Component<CCollider>();
    m_pDetectCollider->Set_ColTag(ColliderTag::NONE);
    m_pDetectCollider->Set_ColType(ColliderType::TRIGGER);
    _vec3 detectbound = _vec3{ 1.f, 1.f, 1.f } * m_fDetectDist;
    m_pDetectCollider->Set_Offset(detectbound);
    m_pDetectCollider->Set_BoundType(BoundingType::AABB);

    Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
    m_pPick = Get_Component<CPickTarget>();


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
    m_vecMetal.clear();
    auto DetectedObj = m_pDetectCollider->Get_Other()->m_pOwner;
    if (typeid(DetectedObj) == typeid(CMetalCube))
    {
        auto Metal = static_cast<CMetalCube*>(DetectedObj);
        m_vecMetal.push_back(Metal);
    }

    for (auto& iter : m_vecMetal)
    {
        iter->MovetoMagnetic(this);
    }
}
//
//void CMagneticCube::MagneticMove()
//{
//}

//void CMagneticCube::DetectMagnetic()
//{
//    m_vecMetal.clear();
//
//    const BoundInfo& myBound = m_pCollider->Get_Bound();
//
//    //for (auto* other : s_AllMagnets)
//    //{
//    //    if (other == this)
//    //        continue;
//
//    //    // other 의 OBB 정보
//    //    CCollider* oCol = other->Get_Component<CCollider>();
//    //    const BoundInfo& otherBound = oCol->Get_Bound();
//
//    //    // SAT 기반 OBB 교차 검사 (push 벡터는 필요 없으면 무시)
//    //    _vec3 dummyPush;
//    //    if (m_pCollider->Calc_Push_OBB(myBound, otherBound, dummyPush))
//    //    {
//    //        m_vecNearMag.push_back(other);
//    //    }
//    //}
//}