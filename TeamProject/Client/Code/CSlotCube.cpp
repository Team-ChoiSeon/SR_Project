#pragma once
#include "pch.h"
#include "CSlotCube.h"
#include "CRigidBody.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CPickTarget.h"
#include "CSlotSensor.h"
#include "CInputMgr.h"
#include "CPickingMgr.h"
#include "CMainPlayer.h"
#include "CFloatingCube.h"
#include "CFactory.h"
#include "Engine_GUI.h"
#include "CGuiSystem.h"
CSlotCube* CSlotCube::s_pPickedCube = nullptr;
CSlotCube::CSlotCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CSlotCube::CSlotCube(const CSlotCube& rhs)
	:CCube(rhs.m_pGraphicDev)
{
}

CSlotCube::~CSlotCube()
{
}

HRESULT CSlotCube::Ready_GameObject()
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
	m_pRigid->Set_Friction(0.1f);
	m_pRigid->Set_Mass(1.f);
	m_pRigid->Set_Bounce(0.1f);
	m_pRigid->Set_OnGround(false);
	m_pRigid->Set_UseGravity(true);

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);
	m_pCollider->Set_BoundType(BoundingType::OBB);

	Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
	m_pPick = Get_Component<CPickTarget>();
	m_FitSlot = nullptr;
	
	m_bFirstPick = true;

	CFactory::Save_Prefab(this, "CSlotCube");
	return S_OK;
}

_int CSlotCube::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bCurGrab)
	{
		if (s_pPickedCube == nullptr) {
			s_pPickedCube = this;
		}
		else if (s_pPickedCube != this) {
			m_bCurGrab = false;
		}
	}
	else
	{
		if (s_pPickedCube == this)
			s_pPickedCube = nullptr;
	}
	if (m_bCurGrab)
	{
		PickMove();
	}	
	else
	{
		if (Check_Overlap()) {
			Fit(fTimeDelta);
		}
		else if (m_FitSlot != nullptr)
			m_FitSlot->Set_SlottedCube(nullptr);
		m_pRigid->Set_UseGravity(true);
		//m_pRigid->Set_OnGround(false);
		m_bFirstPick = true;
	}
	CGameObject::Update_GameObject(fTimeDelta);
	

	return _int();
}

void CSlotCube::LateUpdate_GameObject(const _float& fTimeDelta)
{


	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CSlotCube* CSlotCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSlotCube* sCube = new CSlotCube(pGraphicDev);
	if (FAILED(sCube->Ready_GameObject()))
	{
		Safe_Release(sCube);
		MSG_BOX("SlotCube Create Failed");
		return nullptr;
	}
	return sCube;
}

void CSlotCube::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pRigid);
	Safe_Release(m_pCollider);
	Safe_Release(m_pPick);
	Safe_Release(m_pGraphicDev);
}

void CSlotCube::Set_Info(CMainPlayer* mainplayer, const _int puzzleID, const _int slotID)
{
	m_pPlayer = mainplayer;
	m_iPuzzleID = puzzleID;
	m_iSlotID = slotID;
}


void CSlotCube::Insert_Overlap(CSlotSensor* sensor, _float dist)
{
	m_vecDetected_Slot.insert(make_pair(sensor, dist));
}

void CSlotCube::PickMove()
{
	if (m_bFirstPick)
	{
		m_vDist = m_pPlayer->Get_Component<CTransform>()->Get_Pos() - m_pTransform->Get_Pos();
		m_fDist = D3DXVec3Length(&m_vDist);
		m_bFirstPick = false;
	}
	m_pRigid->Set_UseGravity(false);
	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f }); 

	auto ray = CPickingMgr::Get_Instance()->Get_Ray();
	_vec3 MovedPos = m_pPlayer->GetPos() + ray->_direction* m_fDist;
	
	m_pTransform->Set_Pos(MovedPos);

	m_FitSlot = nullptr;
	m_vecDetected_Slot.clear();
}

_bool CSlotCube::Check_Overlap()
{
	//오버랩된 센서 리스트들 중 거리가 가장 짧은 센서의 Set_Axis 호출
	if (m_vecDetected_Slot.empty())
		return false;

	auto Slotiter = std::min_element(
		m_vecDetected_Slot.begin(),
		m_vecDetected_Slot.end(),
		[](auto const& a, auto const& b) {
			return a.second < b.second;
		});

	CSlotSensor* FitSlot = Slotiter->first;
	m_FitSlot = FitSlot;
	return true;
}

void CSlotCube::Fit(const _float& fTimeDelta)
{
	m_FitSlot->Set_SlottedCube(this);

	m_pTransform->Set_Pos(m_FitSlot->Get_Component<CTransform>()->Get_Pos());
	m_pRigid->Set_UseGravity(false);
	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f }); 


	_float AllignSpeed = 3.f;
	_vec3 SensorLook = m_FitSlot->Get_Look();
	_vec3 CubeLook = m_pTransform->Get_Info(INFO_LOOK);



	_vec3 axis;
	D3DXVec3Cross(&axis, &CubeLook, &SensorLook);



	 //벡터가 거의 일치(혹은 반대)인 경우 처리
	float axisLen2 = D3DXVec3LengthSq(&axis);
	if (axisLen2 < 1e-6f)
		return;
	D3DXVec3Normalize(&axis, &axis);

	// 코사인으로부터 회전 각도 구하기
	float cosA = D3DXVec3Dot(&CubeLook, &SensorLook);
	cosA = cosf(max(-1.f, min(1.f, cosA)));  // clamp
	float fullAngle = acosf(cosA);

	// 실제 적용할 회전량 (step)
	float step = AllignSpeed * fTimeDelta;
	if (step > fullAngle)
		step = fullAngle;

	// Transform에 축회전 누적 적용
	m_pTransform->Rotate_Axis(axis, step);


}


REGISTER_GAMEOBJECT(CSlotCube)