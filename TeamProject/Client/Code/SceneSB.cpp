#pragma once
#include "pch.h"
#include "SceneSB.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCameraMgr.h"

#include "CCollisionMgr.h"

SceneSB::SceneSB(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

SceneSB::~SceneSB()
{
}

HRESULT SceneSB::Ready_Scene()
{
	OutputDebugString(L"SceneSB::Ready_Scene\n");
	Init_Layers();
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	CMonster* pDummy = CMonster::Create(m_pGraphicDev);
	
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"MyPlayer", pPlayer);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"MyDummy", pDummy);
	
	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	// 임시 카메라
	D3DXMATRIX matView, matProj;
	D3DXVECTOR3 vEye(0.f, 0.f, -20.f);   // 카메라 위치
	D3DXVECTOR3 vAt(0.f, 0.f, 0.f);      // 바라보는 지점
	D3DXVECTOR3 vUp(0.f, 1.f, 0.f);      // 위쪽 방향

	D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matView);

	D3DXMatrixPerspectiveFovLH(&matProj,
		D3DXToRadian(60.f),             // 시야각
		WINCX / (float)WINCY,           // 종횡비
		0.1f,                           // near plane
		1000.f);                        // far plane

	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matProj);
	// 임시 카메라 끝

	return S_OK;
}

_int SceneSB::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);


	CCollisionMgr::Get_Instance()->Update_Collision();

	return 0;
}

void SceneSB::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);


}

SceneSB* SceneSB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneSB* pScene = new SceneSB(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneSB::Free()
{
	CScene::Free();
	CCollisionMgr::Get_Instance()->Destroy_Instance();
}