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
	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);

	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });
	m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_bGoBack = false;
	m_bBackward = false;
	m_bOn = true;
    return S_OK;
}

_int CFloatingCube::Update_GameObject(const _float& fTimeDelta)
{
	if (m_bGoBack)
	{


	}
	else 
	{
		if (m_bOn)
		{
			Move(fTimeDelta);
		}
	}
	
    return _int();
}

void CFloatingCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
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
}

void CFloatingCube::ComputeEndPos()
{
	D3DXVec3Normalize(&m_vDirection, &m_vDirection);
	m_vEndPos = m_vStartPos + m_vDirection * m_fMaxDistance;
}

void CFloatingCube::Move(const _float& fTimeDelta)
{
	_vec3 vNowPos = m_pTransform->Get_Pos();
	if (vNowPos - m_vStartPos < m_vEndPos - m_vStartPos)
	{
		m_pTransform->Move_Pos(&m_vDirection, m_fSpeed, fTimeDelta);
	}

}

void CFloatingCube::Sleep()
{
	
}
