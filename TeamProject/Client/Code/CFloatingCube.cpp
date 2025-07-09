#pragma once
#include "pch.h"
#include "CCube.h"
#include "CFloatingCube.h"
#include "CCollider.h"
#include "CGraphicDev.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CRigidBody.h"

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
	DefaultCubeModel tModel;
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CTransform>());
	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CRigidBody>());

	m_pModel = Get_Component<CModel>();
	m_pCollider = Get_Component<CCollider>();
	m_pTransform = Get_Component<CTransform>();
	m_pRigid = Get_Component<CRigidBody>();
	m_pTransform->Set_Scale({ 1.f, 0.3f, 1.f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });

	m_bGoBack = false;	
	m_bBackward = false;
	m_bOn = true;
	m_fSpeed = 1.f;
	m_fSleepTime = 0.f;

    return S_OK;
}

_int CFloatingCube::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bBackward)
	{
		if (m_bGoBack)
		{
			if (m_bOn)
				MoveBack(fTimeDelta);
			else
				Sleep(fTimeDelta);
		}
		else
		{
			if (m_bOn)
				Move(fTimeDelta);
			else
				Sleep(fTimeDelta);
		}
	}
	else
	{
		if (m_bOn)
			Move(fTimeDelta);
	}

	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	return S_OK;
}

void CFloatingCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
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
	Safe_Release(m_pGraphicDev);
	Safe_Release(m_pRigid);
}

void CFloatingCube::Set_Info(const _vec3& vStartPos, const _vec3& vDirection, const _float& fMax, const _float& fSpeed, const _float& SleepTime)
{
	m_vStartPos = vStartPos;
	m_pTransform->Set_Pos(m_vStartPos);
	m_vDirection = vDirection;
	m_fMaxDistance = fMax;
	m_fSpeed = fSpeed;
	m_fSleepTime = SleepTime;
	ComputeEndPos();
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
	_vec3 vConstStartGap = m_vConstVelPos - m_vStartPos;
	_vec3 vDescStartGap = m_vDescVelPos - m_vStartPos;
	_vec3 vEndStartGap = m_vEndPos - m_vStartPos;

	float ForceAdd = (m_fSpeed * m_fSpeed) * m_pRigid->Get_Mass() / (2.0f * D3DXVec3Length(&vConstStartGap));
	//float  decelMag = -(vmax * vmax) / (2.0f * distEndSeg);

	//_vec3 standardvel = m_vDirection * m_fSpeed;

	if (D3DXVec3Length(&vNowStartGap) <= D3DXVec3Length(&vConstStartGap))
	{
		m_pRigid->Set_Force(ForceAdd * m_vDirection);
	}
	else if (D3DXVec3Length(&vNowStartGap) <= D3DXVec3Length(&vDescStartGap))
	{
		m_pRigid->Set_Force({0.f, 0.f, 0.f});
	}
	else if (D3DXVec3Length(&vNowStartGap) <= D3DXVec3Length(&vEndStartGap))
	{
		m_pRigid->Set_Force(ForceAdd * -m_vDirection);

		_vec3 nowvel = m_pRigid->Get_Velocity();
		if (D3DXVec3Length(&nowvel) < 0.001f) {
			m_bGoBack = true;
			m_bOn = false;
		}
	}

}

void CFloatingCube::MoveBack(const _float& fTimeDelta)
{
	_vec3 vNowPos = m_pTransform->Get_Pos();
	_vec3 vNowEndGap = vNowPos - m_vEndPos;
	_vec3 vConstEndGap = m_vConstVelPos - m_vEndPos;
	_vec3 vDescEndGap = m_vDescVelPos - m_vEndPos;
	_vec3 vStartEndGap = m_vStartPos - m_vEndPos;

	float ForceAdd = (m_fSpeed * m_fSpeed) * m_pRigid->Get_Mass() / (2.0f * D3DXVec3Length(&vDescEndGap));
	//float  decelMag = -(vmax * vmax) / (2.0f * distEndSeg);

	//_vec3 standardvel = m_vDirection * m_fSpeed;

	if (D3DXVec3Length(&vNowEndGap) <= D3DXVec3Length(&vDescEndGap))
	{
		m_pRigid->Set_Force(ForceAdd * -m_vDirection);
	}
	else if (D3DXVec3Length(&vNowEndGap) <= D3DXVec3Length(&vConstEndGap))
	{
		m_pRigid->Set_Force({ 0.f, 0.f, 0.f });
	}
	else if (D3DXVec3Length(&vNowEndGap) <= D3DXVec3Length(&vStartEndGap))
	{
		m_pRigid->Set_Force(ForceAdd * m_vDirection);

		_vec3 nowvel = m_pRigid->Get_Velocity();
		if (D3DXVec3Length(&nowvel) < 0.001f) {
			m_bGoBack = false;
			m_bOn = false;
		}
	}
}

void CFloatingCube::Sleep(const _float fTimeDelta)
{
	m_fTime += fTimeDelta;
	if (m_fTime >= m_fSleepTime)
	{
		m_bOn = true;
		m_fTime = 0.f;
	}
}
