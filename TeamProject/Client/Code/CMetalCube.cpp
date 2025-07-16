#pragma once
#include "pch.h"
#include "CMetalCube.h"
#include "CMagneticCube.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CMainPlayer.h"
#include "CFactory.h"

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


    CFactory::Save_Prefab(this, "CMetalCube");
    return S_OK;
}

_int CMetalCube::Update_GameObject(const _float& fTimeDelta)
{
    if (m_bIdle){
        DetectMagnetic(fTimeDelta);
    }
    else if (m_bApproach) {
        ApproachtoMagnetic(fTimeDelta);
    }
    else if (m_bSync) {
        SyncMagnetic(fTimeDelta);
    }
    else if (m_bDetach) {
        DetachMagnetic(fTimeDelta);
    }

    CGameObject::Update_GameObject(fTimeDelta);
    return _int();
}

void CMetalCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMetalCube::Set_Info(CMainPlayer* player)
{
    m_pPlayer = player;
    m_MZone._min = m_pTransform->Get_Pos() - m_pTransform->Get_Scale() * 10;
    m_MZone._max = m_pTransform->Get_Pos() + m_pTransform->Get_Scale() * 10;
}

CMetalCube* CMetalCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CMetalCube* meCube = new CMetalCube(pGraphicDev);
    if (FAILED(meCube->Ready_GameObject()))
    {   
        Safe_Release(meCube);
        MSG_BOX("MeCube Create Failed");
        return nullptr;
    }
    return meCube;
}

void CMetalCube::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pModel);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCollider);
    Safe_Release(m_pGraphicDev);
}

void CMetalCube::DetectMagnetic(const _float& fTimeDelta)
{
    if ((m_pPickObj = m_pPlayer->Get_PickObj()) &&
        (typeid(*m_pPickObj) == typeid(CMagneticCube)) &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().x > m_MZone._min.x &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().y > m_MZone._min.y &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().z > m_MZone._min.z &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().x < m_MZone._max.x &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().y < m_MZone._max.y &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().z < m_MZone._max.z){
        if (!m_bApproach) {
            m_pParentMagnet = m_pPickObj;
            m_bApproach = true;
            m_bIdle = false;
        }
    }
    else
    {
        m_pRigid->Set_UseGravity(true);
        m_pParentMagnet = nullptr;
        m_pPickMagnet = nullptr;
        m_pPickObj = nullptr;
    }
}

void CMetalCube::ApproachtoMagnetic(const _float& fTimeDelta)
{
    m_pRigid->Set_UseGravity(false);
    m_vParentPos = m_pParentMagnet->Get_Component<CTransform>()->Get_Pos();
    m_vGap = m_vParentPos - m_pTransform->Get_Pos();
    m_fGap = D3DXVec3Length(&m_vGap);


    if (m_pCollider->Get_ColState() == ColliderState::ENTER ||
        m_pCollider->Get_ColState() == ColliderState::STAY)
    {
        if (m_pCollider->Get_Other()->m_pOwner == m_pParentMagnet || 
            typeid(*(m_pCollider->Get_Other()->m_pOwner)) == typeid(CMetalCube))
        {
            m_vSyncGap = m_vParentPos - m_pTransform->Get_Pos();
            m_fSyncGap = D3DXVec3Length(&m_vSyncGap);
            m_bSync = true;
            m_bApproach = false;
        }
    }
    else
        m_pTransform->Move_Pos(&m_vGap, 1.5f, fTimeDelta);


}

void CMetalCube::SyncMagnetic(const _float& fTimeDelta)
{
    if(m_pPlayer->Get_Hold())
    {
        m_pRigid->Set_UseGravity(false);
        m_vParentPos = m_pParentMagnet->Get_Component<CTransform>()->Get_Pos();

        _vec3 syncPos = m_vParentPos - m_vSyncGap;

        m_pTransform->Set_Pos(syncPos);

        m_pCollider->Set_ColType(ColliderType::ACTIVE);
    }
    else if (m_pPlayer->Get_MouseAway())
    {
        //m_pCollider->Set_ColType(ColliderType::PASSIVE);
        m_bDetach = true;
        m_bSync = false;
    }
}

void CMetalCube::DetachMagnetic(const _float& fTimeDelta)
{
    m_pRigid->Set_UseGravity(true);
    m_pRigid->Set_OnGround(false);
    if (m_pPlayer->Get_MouseTap())
    {
        m_bDetach = false;
        m_bIdle = true;
    }
    else
    {
        m_pParentMagnet = nullptr;
        m_pPickMagnet = nullptr;
        m_pPickObj = nullptr;
        m_vSyncGap = { 0, 0, 0 };
        m_vGap = { 0, 0, 0 };
    }
}

REGISTER_GAMEOBJECT(CMetalCube)