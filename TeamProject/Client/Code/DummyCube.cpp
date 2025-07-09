#pragma once
#include "pch.h"
#include "DummyCube.h"
#include "CGameObject.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CCollisionMgr.h"
#include "CModel.h"
#include "CInputMgr.h"
#include "Engine_Model.h"
#include "CPickTarget.h"
#include "CPickingMgr.h"

#include "Engine_Macro.h"
#include "CFactory.h"


DummyCube::DummyCube(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CGameObject(pGraphicDev)
{ 
}

DummyCube::~DummyCube()
{
}

HRESULT DummyCube::Ready_GameObject()
{
	DefaultCubeModel tModel;
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	//Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);

	m_pModel = Get_Component<CModel>();
	m_pCollider = Get_Component<CCollider>();
	m_pTransform = Get_Component<CTransform>();
	m_pPick = Get_Component<CPickTarget>();

	m_pTransform->Set_Pos({ 0.f, 0.f, 10.f });
	m_pTransform->Set_Look({ 0.f, 0.f, -1.f });
	m_pTransform->Set_Right({ -1.f, 0.f, 0.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Angle({ 0.f, D3DX_PI, 0.f });
	CFactory::Save_Prefab(this, "DummyCube");

	return S_OK;
}

int DummyCube::Update_GameObject(const _float& fTimeDelta)
{
	Key_Input(fTimeDelta);
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	return 0;
}

void DummyCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

DummyCube* DummyCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	DummyCube* pDummy = new DummyCube(pGraphicDev);

	if (FAILED(pDummy->Ready_GameObject()))
	{
		Safe_Release(pDummy);
		MSG_BOX("Dummy Create Failed");
		return nullptr;
	}

	return pDummy;
}

void DummyCube::Free()
{
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
	Safe_Release(m_pPick);
}

void DummyCube::Key_Input(const float& fTimeDelta)
{
	float fAngleSpeed = 5.f;
	if (CInputMgr::Get_Instance()->Key_Down(DIK_UP)) {
		m_pTransform->Set_Angle(m_pTransform->Get_Angle() + _vec3{ -fAngleSpeed * fTimeDelta, 0.f, 0.f });
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN)) {
		m_pTransform->Set_Angle(m_pTransform->Get_Angle() + _vec3{ fAngleSpeed * fTimeDelta, 0.f, 0.f });
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT)) {
		m_pTransform->Set_Angle(m_pTransform->Get_Angle() + _vec3{ 0.f, fAngleSpeed * fTimeDelta, 0.f });
	}
	if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT)) {
		m_pTransform->Set_Angle(m_pTransform->Get_Angle() + _vec3{ 0.f, -fAngleSpeed * fTimeDelta, 0.f });
	}
}

