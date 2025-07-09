#pragma once
#include "pch.h"
#include "CCube.h"
#include "CFloatingCube.h"
#include "CCollider.h"
#include "CGraphicDev.h"
#include "CGameObject.h"
#include "CTransform.h"

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
	//Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);

	m_pModel = Get_Component<CModel>();
	m_pCollider = Get_Component<CCollider>();
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Set_Scale({ 1.f, 0.3f, 1.f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_bGoBack = false;
	m_bBackward = false;
	m_bOn = true;
	m_fSpeed = 1.f;
	m_fSleepTime = 2.f;

    return S_OK;
}

_int CFloatingCube::Update_GameObject(const _float& fTimeDelta)
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
}

void CFloatingCube::Move(const _float& fTimeDelta)
{
	_vec3 vNowPos = m_pTransform->Get_Pos();
	_vec3 vNowGap = vNowPos - m_vStartPos;
	_vec3 vEndGap = m_vEndPos - m_vStartPos;

	if (D3DXVec3Length(&vNowGap) <= D3DXVec3Length(&vEndGap))
	{
		m_pTransform->Move_Pos(&m_vDirection, m_fSpeed, fTimeDelta);
	}
	else {
		m_bGoBack = true;
		m_bOn = false;
	}

}

void CFloatingCube::MoveBack(const _float& fTimeDelta)
{
	_vec3 vNowPos = m_pTransform->Get_Pos();
	_vec3 vNowGap = vNowPos - m_vEndPos;
	_vec3 vStartPos = m_vStartPos - m_vEndPos;

	if (D3DXVec3Length(&vNowGap) <= D3DXVec3Length(&vStartPos))
	{
		m_pTransform->Move_Pos(&m_vDirection, -m_fSpeed, fTimeDelta);
	}
	else {
		m_bGoBack = false;
		m_bOn = false;
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
