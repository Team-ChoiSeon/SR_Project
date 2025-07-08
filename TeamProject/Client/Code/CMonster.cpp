#pragma once
#include "pch.h"
#include "CMonster.h"
#include "CInputMgr.h"
#include "CCollisionMgr.h"
#include "Engine_Model.h"

CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}
CMonster::~CMonster()
{
}

HRESULT CMonster::Ready_GameObject()
{

	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Transform();
	m_pTransform->Set_Pos({ 1.f, -2.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	DefaultCubeModel tModel;
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	m_pModel = Get_Component<CModel>();

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev);
	m_pCollider = Get_Component<CCollider>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();
	m_pCollider->Set_ColTag(ColliderTag::NONE);
	m_pCollider->Set_ColType(ColliderType::ACTIVE);

	return S_OK;
}

int CMonster::Update_GameObject(const _float& fTimeDelta)
{

	KeyInput(fTimeDelta);
	// For debug
	wstring wDebug = to_wstring(m_pTransform->Get_Pos().z);
	OutputDebugString(wDebug.c_str());
	OutputDebugString(L"\n");

	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);


	return S_OK;
}

void CMonster::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

void CMonster::KeyInput(const _float& fTimeDelta)
{

	if (CInputMgr::Get_Instance()->Key_Down(DIK_UP))
		m_pRigid->Add_Force({ 0.f, 0.f, 100.f });

	if (CInputMgr::Get_Instance()->Key_Down(DIK_DOWN))
		m_pRigid->Add_Force({ 0.f, 0.f, -100.f });

	if (CInputMgr::Get_Instance()->Key_Down(DIK_RIGHT))
		m_pRigid->Add_Force({ 100.f, 0.f, 0.f });

	if (CInputMgr::Get_Instance()->Key_Down(DIK_LEFT))
		m_pRigid->Add_Force({ -100.f, 0.f, 0.f });

}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CMonster* pMonster = new CMonster(pGraphicDev);

	if (FAILED(pMonster->Ready_GameObject()))
	{
		Safe_Release(pMonster);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pMonster;
}

void CMonster::Free()
{
	Safe_Release(m_pRigid);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
}