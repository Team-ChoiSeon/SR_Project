#pragma once
#include "pch.h"
#include "DummyCube.h"
#include "CGameObject.h"
#include "CRcCube.h"
#include "CTransform.h"

DummyCube::DummyCube(LPDIRECT3DDEVICE9 pGrpahicDev) : CGameObject(pGrpahicDev)
{
	m_pGraphicDev = pGrpahicDev;
	Add_Component<CRcCube>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);

}

DummyCube::~DummyCube()
{
}

HRESULT DummyCube::Ready_GameObject()
{
	m_pCube = Get_Component<CRcCube>();
	m_pTransform = Get_Component<CTransform>();

	m_pTransform->Ready_Transform();
	m_pTransform->Set_Pos({ 0.f, 0.f, 10.f });
	m_pTransform->Set_Look({ 0.f, 0.f, -1.f });
	m_pTransform->Set_Right({ -1.f, 0.f, 0.f });
	m_pTransform->Set_Up({ 0.f, 1.f, 0.f });
	m_pTransform->Set_Angle({ 0.f, D3DX_PI, 0.f });
	m_pCube->Ready_Buffer();

	return S_OK;
}

int DummyCube::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);


	return 0;
}

void DummyCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->LateUpdate_Component();
}

void DummyCube::Render_GameObject()
{
	m_mWorld = Get_Component<CTransform>()->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_mWorld);
	Get_Component<CRcCube>()->Render_Buffer();
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
	Safe_Release(m_pCube);
	Safe_Release(m_pTransform);
}
