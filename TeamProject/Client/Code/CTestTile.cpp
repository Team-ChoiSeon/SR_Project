#pragma once
#include "pch.h"
#include "CTestTile.h"

#include "CFactory.h"

CTestTile::CTestTile(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{

}

CTestTile::~CTestTile()
{

}

HRESULT CTestTile::Ready_GameObject()
{
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Get_Component<CTransform>();
	m_pTransform->Ready_Transform();
	m_pTransform->Set_Pos({ 1.f, -2.f, 0.f });
	m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Right({ 1.f, 0.f, 0.f });

	DefaultCubeModel tModel;
	tModel.meshKey = L"DirtObj.obj";
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev, tModel);
	m_pModel = Get_Component<CModel>();

	Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
	m_pRigid = Get_Component<CRigidBody>();

	Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
	m_pCollider = Get_Component<CCollider>();
	m_pCollider->Set_ColTag(ColliderTag::GROUND);
	m_pCollider->Set_ColType(ColliderType::PASSIVE);


	CFactory::Save_Prefab(this, "CTestTile");

	return S_OK;
}

int CTestTile::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);


	return 0;
}

void CTestTile::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

CTestTile* CTestTile::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CTestTile* pTile = new CTestTile(pGraphicDev);

	if (FAILED(pTile->Ready_GameObject()))
	{
		Safe_Release(pTile);
		MSG_BOX("Monster Create Failed");
		return nullptr;
	}

	return pTile;
}
void CTestTile::Free()
{
	Safe_Release(m_pRigid);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
	Safe_Release(m_pTransform);
}

REGISTER_GAMEOBJECT(CTestTile)