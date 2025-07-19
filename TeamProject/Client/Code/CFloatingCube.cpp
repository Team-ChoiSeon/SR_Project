#pragma once
#include "pch.h"
#include "CCube.h"
#include "CFloatingCube.h"
#include "CCollider.h"
#include "CGraphicDev.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidBody.h"
#include "CFactory.h"

CFloatingCube::CFloatingCube(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CFloatingCube::CFloatingCube(const CFloatingCube& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CFloatingCube::~CFloatingCube()
{
}

HRESULT CFloatingCube::Ready_GameObject()
{
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
	m_pModel = Get_Component<CModel>();

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Component();
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });

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
	m_pCollider->Set_ColType(ColliderType::PASSIVE);
	m_pCollider->Set_BoundType(BoundingType::AABB);

	m_bGoBack = false;	
	m_bBackward = false;
	m_bOn = false;
	m_bSleep = false;
	m_fSpeed = 1.f;
	m_fSleepTime = 0.f;

	m_vPrePos = { 0.f, 0.f, 0.f };

	CFactory::Save_Prefab(this, "CFloatingCube");
    return S_OK;
}

_int CFloatingCube::Update_GameObject(const _float& fTimeDelta)
{

	if (m_bOn) {
		if (m_bBackward)
		{
			if (m_bGoBack)
			{
				if (!m_bSleep)
					MoveBack(fTimeDelta);
				else
				{
					Sleep(fTimeDelta);
				}
			}
			else
			{
				if (!m_bSleep)
					Move(fTimeDelta);
				else
				{
					Sleep(fTimeDelta);
				}
			}
		}
		else if(!m_bGoBack)
		{
			if (!m_bSleep)
				Move(fTimeDelta);
			else
				Sleep(fTimeDelta);
		}
	}
	else
		Stop(fTimeDelta);

	SyncVelPlayer(fTimeDelta);


	m_vPrePos = m_pTransform->Get_Pos();

	CGameObject::Update_GameObject(fTimeDelta);

	return S_OK;
}

void CFloatingCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CFloatingCube* CFloatingCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{  
   CFloatingCube* pFCube = new CFloatingCube(pGraphicDev);  
   if (FAILED(pFCube->Ready_GameObject()))
   {  
       Safe_Release( pFCube);
	   MSG_BOX("FCube Create Failed");
	   return nullptr;
   }  
   return pFCube;
}

void CFloatingCube::Free()
{
	Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pRigid);
}

void CFloatingCube::Set_Info(const _vec3& vDirection, const _float& fMax, const _float& fSpeed, const _float& SleepTime)
{
	m_vStartPos = m_pTransform->Get_Pos();
	m_vDirection = vDirection;
	m_fMaxDistance = fMax;
	m_fSpeed = fSpeed;
	m_fSleepTime = SleepTime;
	m_bBackward = false;
	m_bSleep = true;
	ComputeEndPos();

}

void CFloatingCube::SyncVelPlayer(const _float& fTimeDelta)
{
	CCollider* pOtherCol = m_pCollider->Get_Other();
	if (pOtherCol)
	{
		if ((pOtherCol->m_pOwner->Get_Component<CTransform>()->Get_Pos().y - pOtherCol->m_pOwner->Get_Component<CTransform>()->Get_Scale().y) >
			(m_pTransform->Get_Pos().y + m_pTransform->Get_Scale().y - 0.1f))
		{
			//pOtherCol->m_pOwner->Get_Component<CTransform>()->Move_Pos(&m_vDirection, m_fSpeed , fTimeDelta);
			_vec3 Nowpos = m_pTransform->Get_Pos();
			_vec3 Gap = Nowpos - m_vPrePos;
			pOtherCol->m_pOwner->Get_Component<CTransform>()->Set_Pos(pOtherCol->m_pOwner->Get_Component<CTransform>()->Get_Pos() + Gap);
			
			//Rigid Sync
			//pOtherCol->m_pOwner->Get_Component<CRigidBody>()->Set_Velocity(m_pRigid->Get_Velocity());
		}
	}
}

void CFloatingCube::ComputeEndPos()
{
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	m_vEndPos = m_vStartPos + m_vDirection * m_fMaxDistance;
	m_vConstVelPos = m_vStartPos + m_vDirection * m_fMaxDistance * 0.2f;
	m_vDescVelPos = m_vStartPos + m_vDirection * m_fMaxDistance * 0.8f;
}


void CFloatingCube::Move(const _float& fTimeDelta)
{
	_vec3 vNowPos = m_pTransform->Get_Pos();
	_vec3 vNowStartGap = vNowPos - m_vStartPos;
	_vec3 vEndStartGap = m_vEndPos - m_vStartPos;
	_vec3 vConstStartGap = m_vConstVelPos - m_vStartPos;
	_vec3 vDescStartGap = m_vDescVelPos - m_vStartPos;

	_float fTravelDist = D3DXVec3Length(&vNowStartGap);
	_float fTotalDist = D3DXVec3Length(&vEndStartGap);

	//=============================== Move by Transfrom ===============================//
	if (fTravelDist < fTotalDist)
	{
		m_pTransform->Move_Pos(&m_vDirection, m_fSpeed, fTimeDelta);
		_vec3 MovedGap = m_pTransform->Get_Pos() - m_vStartPos;
		_float MovedDist = D3DXVec3Length(&MovedGap);
		if (MovedDist > fTotalDist)
			m_pTransform->Set_Pos(m_vEndPos);
	}
	else
	{
		m_pTransform->Set_Pos(m_vEndPos);
		m_bGoBack = true;
		m_bSleep = true;
	}
	//=============================== Move by Rigid ===============================//

	//_float fConstDist = D3DXVec3Length(&vConstStartGap);
	//_float fDescDist = D3DXVec3Length(&vDescStartGap);

	//_float Accel = ((m_fSpeed * m_fSpeed) / (2.f * fConstDist));

	//if (fTravelDist < fConstDist)
	//{
	//	m_pRigid->Add_Velocity(Accel * m_vDirection *fTimeDelta);
	//}
	//else if (fTravelDist < fDescDist)
	//{
	//	m_pRigid->Add_Velocity(0.f * m_vDirection * fTimeDelta);
	//}
	//else if (fTravelDist < fTotalDist)
	//{

	//	m_pRigid->Add_Velocity(-Accel * m_vDirection * fTimeDelta);
	//}
	//else
	//{
	//	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
	//	m_bGoBack = true;
	//	m_bSleep = true;
	//}
}

void CFloatingCube::MoveBack(const _float& fTimeDelta)
{
	_vec3 vNowPos = m_pTransform->Get_Pos();
	_vec3 vNowStartGap = vNowPos - m_vEndPos;
	_vec3 vEndStartGap = m_vStartPos - m_vEndPos;
	_vec3 vConstStartGap = m_vConstVelPos - m_vEndPos;
	_vec3 vDescStartGap = m_vDescVelPos - m_vEndPos;

	_float fTravelDist = D3DXVec3Length(&vNowStartGap);
	_float fTotalDist = D3DXVec3Length(&vEndStartGap);

	//=============================== Move by Transfrom ===============================//

	if (fTravelDist < fTotalDist)
	{
		m_pTransform->Move_Pos(&m_vDirection, -m_fSpeed, fTimeDelta);
		_vec3 MovedGap = m_pTransform->Get_Pos() - m_vEndPos;
		_float MovedDist = D3DXVec3Length(&MovedGap);
		if (MovedDist > fTotalDist)
			m_pTransform->Set_Pos(m_vStartPos);
	}
	else
	{
		m_pTransform->Set_Pos(m_vStartPos);
		m_bGoBack = false;
		m_bSleep = true;
	}

	//=============================== Move by Rigid ===============================//

	//_float fConstDist = D3DXVec3Length(&vConstStartGap);
	//_float fDescDist = D3DXVec3Length(&vDescStartGap);

	//_float Accel = ((m_fSpeed * m_fSpeed) / (2.f * fDescDist));

	//if (fTravelDist < fDescDist)
	//{
	//	m_pRigid->Add_Velocity(-Accel * m_vDirection * fTimeDelta);
	//}
	//else if (fTravelDist < fConstDist)
	//{
	//	m_pRigid->Add_Velocity(0.f * m_vDirection * fTimeDelta);
	//}
	//else if (fTravelDist < fTotalDist)
	//{

	//	m_pRigid->Add_Velocity(Accel * m_vDirection * fTimeDelta);
	//}
	//else
	//{
	//	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
	//	m_bGoBack = false;
	//	m_bSleep = true;
	//}

}

void CFloatingCube::Sleep(const _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime >= m_fSleepTime)
	{
		m_bSleep = false;
		m_fTime = 0.f;
	}
}

void CFloatingCube::Stop(const _float& fTimeDelta)
{
	m_pRigid->Set_Velocity({ 0.f, 0.f, 0.f });
	m_pRigid->Set_Force({ 0.f, 0.f, 0.f });
}

REGISTER_GAMEOBJECT(CFloatingCube)