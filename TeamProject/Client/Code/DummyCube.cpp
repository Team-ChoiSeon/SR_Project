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
	
	m_pCube = Get_Component<CRcCube>();

}

DummyCube::~DummyCube()
{
}

HRESULT DummyCube::Ready_Dummy()
{
	auto transform = Get_Component<CTransform>();
	auto cube = Get_Component<CRcCube>();

	transform->Ready_Transform();
	transform->Set_Pos({ 0.f, 0.f, 10.f });
	cube->Ready_Buffer();
	return S_OK;
}

int DummyCube::Update_Dummy(const float& fTimeDelta)
{
	return 0;
}

void DummyCube::LateUpdate_Dummy(const float& fTimeDelta)
{
}

void DummyCube::Render_GameObject()
{
	m_mWorld = Get_Component<CTransform>()->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_mWorld);
	Get_Component<CRcCube>()->Render_Buffer();
}

DummyCube* DummyCube::Create_Dummy(LPDIRECT3DDEVICE9 pGraphicDev)
{
	DummyCube* pDummy = new DummyCube(pGraphicDev);

	if (FAILED(pDummy->Ready_Dummy()))
	{
		Safe_Release(pDummy);
		MSG_BOX("Dummy Create Failed");
		return nullptr;
	}

	return pDummy;
}

void DummyCube::Free()
{
}
