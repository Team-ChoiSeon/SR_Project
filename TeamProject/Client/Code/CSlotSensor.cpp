#pragma once
#include "pch.h"
#include "CSlotSensor.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CCollider.h"
#include "CMainPlayer.h"
#include "CSlotCube.h"
#include "CSlotCube_Auto.h"
#include "CInputMgr.h"
#include "CFactory.h"
CSlotSensor::CSlotSensor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSensor(pGraphicDev), m_pSlotted(nullptr), m_pSlotted_Auto(nullptr)
{
}

CSlotSensor::CSlotSensor(const CSlotSensor& rhs)
	: CSensor(rhs.m_pGraphicDev)
{
}

CSlotSensor::~CSlotSensor()
{
}

HRESULT CSlotSensor::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();
    m_pTransform->Ready_Component();
    m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
    m_pTransform->Set_Angle({ 0.f, 0.25f, 0.f });
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
    m_pCollider->Set_ColTag(ColliderTag::NONE);
    m_pCollider->Set_ColType(ColliderType::TRIGGER);
    m_pCollider->Set_BoundType(BoundingType::OBB);

    m_bSensorOn = false;

    CFactory::Save_Prefab(this, "CSlotSensor"); 
    
    return S_OK;

}

_int CSlotSensor::Update_GameObject(const _float& fTimeDelta)
{
    Insert_Slot();
    CGameObject::Update_GameObject(fTimeDelta);
	return _int();
}

void CSlotSensor::LateUpdate_GameObject(const _float& fTimeDelta)
{
    m_bSensorOn = Detect();
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CSlotSensor* CSlotSensor::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CSlotSensor* sSensor = new CSlotSensor(pGraphicDev);
    if (FAILED(sSensor->Ready_GameObject()))
    {
        Safe_Release(sSensor);
        MSG_BOX("SlotSensor Create Failed");
        return nullptr;
    }
    return sSensor;
}

void CSlotSensor::Free()
{
    Safe_Release(m_pPlayer);
    Safe_Release(m_pTransform);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCollider);
    Safe_Release(m_pGraphicDev);
}

_bool CSlotSensor::Detect()
{
    return Check_Slot();
}

void CSlotSensor::Set_Info(CMainPlayer* player, const _int puzzleID, const _int slotID)
{
    m_pPlayer = player;
    m_iPuzzleID = puzzleID;
    m_iSlotID = slotID;
    m_Zone._min = m_pTransform->Get_Pos() - m_pTransform->Get_Scale() * 2;
    m_Zone._max = m_pTransform->Get_Pos() + m_pTransform->Get_Scale() * 2;
}


void CSlotSensor::Insert_Slot()
{
    if (m_bPlayerPick)
    {
        if (m_pSlotted)
            return;
        if (m_pPickObj = m_pPlayer->Get_PickObj()) {
            m_pPickSlot = dynamic_cast<CSlotCube*>(m_pPickObj);
        }
        else if (m_pPickSlot)
        {
            if (m_pPickSlot->Get_PuzzleID() == m_iPuzzleID &&
                m_pPickSlot->Get_Component<CTransform>()->Get_Pos().x > m_Zone._min.x &&
                m_pPickSlot->Get_Component<CTransform>()->Get_Pos().y > m_Zone._min.y &&
                m_pPickSlot->Get_Component<CTransform>()->Get_Pos().z > m_Zone._min.z &&
                m_pPickSlot->Get_Component<CTransform>()->Get_Pos().x < m_Zone._max.x &&
                m_pPickSlot->Get_Component<CTransform>()->Get_Pos().y < m_Zone._max.y &&
                m_pPickSlot->Get_Component<CTransform>()->Get_Pos().z < m_Zone._max.z)
            {
                _vec3 vDist = m_pPickSlot->Get_Component<CTransform>()->Get_Pos() - m_pTransform->Get_Pos();
                _float fDist = D3DXVec3Length(&vDist);
                m_pPickSlot->Insert_Overlap(this, fDist);
            }
            m_pPickObj = nullptr;
            m_pPickSlot = nullptr;
        }
    }
    else {
        if (m_pSlotted_Auto)
            return;
        
        CCollider* pOtherCol = m_pCollider->Get_Other();
        if (pOtherCol)
        {
            CGameObject* pOtherObj = pOtherCol->m_pOwner;
            if (pOtherObj)
            {
                CSlotCube_Auto* pAutoCube = dynamic_cast<CSlotCube_Auto*>(pOtherObj);
                if (pAutoCube)
                {
                    m_pPickSlot_Auto = pAutoCube;
                }
            }
        }
        else if (m_pPickSlot_Auto){
            if (m_pPickSlot_Auto->Get_PuzzleID() == m_iPuzzleID &&
                m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos().x > m_Zone._min.x &&
                m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos().y > m_Zone._min.y &&
                m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos().z > m_Zone._min.z &&
                m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos().x < m_Zone._max.x &&
                m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos().y < m_Zone._max.y &&
                m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos().z < m_Zone._max.z)
            {
                _vec3 vDist = m_pPickSlot_Auto->Get_Component<CTransform>()->Get_Pos() - m_pTransform->Get_Pos();
                _float fDist = D3DXVec3Length(&vDist);
                m_pPickSlot_Auto->Insert_Overlap(this, fDist);
            }
            m_pPickSlot_Auto = nullptr;
        }
    }
}

bool CSlotSensor::Check_Slot()
{
    if (m_bPlayerPick)
    {
        if (m_pSlotted)
        {
            // 1) 현재 할당된 큐브가 여전히 영역 안에 있나 확인
            _vec3 pos = m_pSlotted->Get_Component<CTransform>()->Get_Pos();
            if (pos.x < m_Zone._min.x || pos.y < m_Zone._min.y || pos.z < m_Zone._min.z ||
                pos.x > m_Zone._max.x || pos.y > m_Zone._max.y || pos.z > m_Zone._max.z)
            {
                // 영역 밖으로 나갔으니 해제
                m_pSlotted = nullptr;
                return false;
            }
            // 2) 여전히 올바른 슬롯 ID인지 확인
            return dynamic_cast<CSlotCube*>(m_pSlotted)->Get_SlotID() == m_iSlotID;
        }
    }
    else {
        if (m_pSlotted_Auto)
        {
            _vec3 pos = m_pSlotted_Auto->Get_Component<CTransform>()->Get_Pos();
            if (pos.x < m_Zone._min.x || pos.y < m_Zone._min.y || pos.z < m_Zone._min.z ||
                pos.x > m_Zone._max.x || pos.y > m_Zone._max.y || pos.z > m_Zone._max.z)
            {
                m_pSlotted_Auto = nullptr;
                return false;
            }
            return dynamic_cast<CSlotCube_Auto*>(m_pSlotted_Auto)->Get_SlotID() == m_iSlotID;
        }
    }
    return false;
}


REGISTER_GAMEOBJECT(CSlotSensor)