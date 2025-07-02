#pragma once
#include "pch.h"
#include "CCameraObject.h"
#include "CTransform.h"
#include "CCamera.h"

CCameraObject::~CCameraObject()
{
}

CCameraObject::CCameraObject(LPDIRECT3DDEVICE9 pGraphicDev) : CGameObject(pGraphicDev)
{
	Add_Component<CTransform>(ID_DYNAMIC,pGraphicDev);
	Add_Component<CCamera>(ID_DYNAMIC, pGraphicDev);
}

CCameraObject::CCameraObject(const CCameraObject& rhs) : CGameObject(rhs.m_pGraphicDev)
{
	Add_Component<CTransform>(ID_DYNAMIC, rhs.m_pGraphicDev);
	Add_Component<CCamera>(ID_DYNAMIC, rhs.m_pGraphicDev);
}

HRESULT CCameraObject::Ready_GameObject()
{
	return S_OK;
}

int CCameraObject::Update_GameObject(const _float& fTimeDelta)
{
	return 0;
}

void CCameraObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
}


void CCameraObject::Render_GameObject()
{
}

void CCameraObject::Free()
{
}

void CCameraObject::ApplyViewMatrix()
{
	auto camera = Get_Component<CCamera>();
	m_pGraphicDev->SetTransform(D3DTS_VIEW, camera->Get_ViewMatrix());
}

void CCameraObject::ApplyProjectionMatrix()
{
	auto camera = Get_Component<CCamera>();
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, camera->Get_ProjectionMatrix());
}
