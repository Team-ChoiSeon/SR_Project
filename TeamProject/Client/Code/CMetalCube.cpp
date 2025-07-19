#pragma once
#include "pch.h"
#include "CMetalCube.h"
#include "CMagneticCube.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CMainPlayer.h"
#include "CFactory.h"
#include "Engine_GUI.h"
#include "CGuiSystem.h"
#include "CTestTile.h"

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
    m_pCollider->Set_ColTag(ColliderTag::GROUND);
    m_pCollider->Set_ColType(ColliderType::ACTIVE);

    m_eState = METAL_STATE::IDLE;
    CFactory::Save_Prefab(this, "CMetalCube");
    return S_OK;
}

_int CMetalCube::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);    
    
    switch (m_eState)
    {
    case METAL_STATE::IDLE:
        DetectMagnetic(fTimeDelta);
        break;
    case METAL_STATE::APPROACH:
        ApproachtoMagnetic(fTimeDelta);
        break;
    case METAL_STATE::SYNC:
        SyncMagnetic(fTimeDelta);
        break;
    case METAL_STATE::DETACH:
        DetachMagnetic(fTimeDelta);
    }


    if (m_pRigid->Get_OnGround())
        m_pCollider->Set_ColType(ColliderType::PASSIVE);
    else {
        m_pCollider->Set_ColType(ColliderType::ACTIVE);
		m_pRigid->Set_UseGravity(true);
		m_pRigid->Set_OnGround(false);
    }

    //Deubbing Code
    CGuiSystem::Get_Instance()->RegisterPanel("state", [this]() {
    	// 간단한 GUI 창 하나 출력
    	ImGui::SetNextWindowSize(ImVec2{ 200,200 });
        switch (m_eState)
        {
        case METAL_STATE::IDLE:
            ImGui::Begin("IDLE");
            break;
        case METAL_STATE::APPROACH:
            ImGui::Begin("APPROACH");
            break;
        case METAL_STATE::SYNC:
            ImGui::Begin("SYNC");
            break;
        case METAL_STATE::DETACH:
            ImGui::Begin("DETACH");
        }
        //if (m_pRigid->Get_OnGround())
        //    ImGui::Begin("On Ground");
        //else if (!m_pRigid->Get_OnGround())
        //    ImGui::Begin("Not On Ground");

    	ImGui::End();

    	});
    return _int();
}

void CMetalCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CMetalCube::Set_Info(CMainPlayer* player)
{
    m_pPlayer = player;
    m_MZone._min = m_pTransform->Get_Pos() - m_pTransform->Get_Scale() * 3;
    m_MZone._max = m_pTransform->Get_Pos() + m_pTransform->Get_Scale() * 3;
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
}

void CMetalCube::DetectMagnetic(const _float& fTimeDelta)
{
    m_MZone._min = m_pTransform->Get_Pos() - m_pTransform->Get_Scale() * 3;
    m_MZone._max = m_pTransform->Get_Pos() + m_pTransform->Get_Scale() * 3;
    if (m_pPlayer->Get_Hold() &&
        (m_pPickObj = m_pPlayer->Get_PickObj()) &&
        (typeid(*m_pPickObj) == typeid(CMagneticCube)) &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().x > m_MZone._min.x &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().y > m_MZone._min.y &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().z > m_MZone._min.z &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().x < m_MZone._max.x &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().y < m_MZone._max.y &&
        m_pPickObj->Get_Component<CTransform>()->Get_Pos().z < m_MZone._max.z){
            m_pParentMagnet = m_pPickObj;
            m_eState = METAL_STATE::APPROACH;
    }
}

void CMetalCube::ApproachtoMagnetic(const _float& fTimeDelta)
{
    m_pRigid->Set_UseGravity(false);
    m_vParentPos = m_pParentMagnet->Get_Component<CTransform>()->Get_Pos();
    m_vGap = m_vParentPos - m_pTransform->Get_Pos();
    if (m_pPlayer->Get_MouseAway())
    {
        m_eState = METAL_STATE::DETACH;
        return;
    }
    else if (m_pCollider->Get_Other()) {
        auto col = m_pCollider->Get_Other()->m_pOwner;
        if (typeid(*col) != typeid(CTestTile) &&
            (m_pCollider->Get_Other()->m_pOwner == m_pParentMagnet ||
            typeid(*col) == typeid(CMetalCube)))
        {
            m_vSyncGap = m_vParentPos - m_pTransform->Get_Pos();
            m_pCollider->Set_ColType(ColliderType::TRIGGER);
            m_eState = METAL_STATE::SYNC;
            return;
        }
    }
    {
        m_pTransform->Move_Pos(&m_vGap, 2.f, fTimeDelta);
    }
}

void CMetalCube::SyncMagnetic(const _float& fTimeDelta)
{
    if (static_cast<CMagneticCube*>(m_pParentMagnet)->Get_Away())
    {
        m_eState = METAL_STATE::DETACH;
        return;
    }
    else if(static_cast<CMagneticCube*>(m_pParentMagnet)->Get_Grab())
    {
        m_pRigid->Set_UseGravity(false);
        m_vParentPos = m_pParentMagnet->Get_Component<CTransform>()->Get_Pos();

        _vec3 syncPos = m_vParentPos - m_vSyncGap;
        m_pTransform->Set_Pos(syncPos);
    }
}

void CMetalCube::DetachMagnetic(const _float& fTimeDelta)
{
    m_pRigid->Set_UseGravity(true);
    m_pRigid->Set_OnGround(false);
    m_pParentMagnet = nullptr;
    m_pPickMagnet = nullptr;
    m_pPickObj = nullptr;
	m_vParentPrePos = { 0, 0, 0 };
    m_vSyncGap = { 0, 0, 0 };
    m_vGap = { 0, 0, 0 };
    m_eState = METAL_STATE::IDLE;
    m_pCollider->Set_ColType(ColliderType::ACTIVE);
}

REGISTER_GAMEOBJECT(CMetalCube)