#pragma once
#include "pch.h"
#include "SceneSB.h"

#include "CPlayer.h"
#include "CMonster.h"
#include "CCameraMgr.h"
#include "CFirstviewFollowingCamera.h"
#include "DummyCube.h"

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

	// 1. 플레이어 (시점 고정)
	CPlayer* pPlayer = CPlayer::Create(m_pGraphicDev);
	pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 0.f, -20.f });

	// 2. 바닥 역할 (몬스터 착지용)
	CMonster* pTile = CMonster::Create(m_pGraphicDev);
	pTile->Get_Component<CTransform>()->Set_Scale({ 100.f, 10.f, 100.f });
	pTile->Get_Component<CTransform>()->Set_PosY(-20.f);
	pTile->Get_Component<CRigidbody>()->Set_OnGround(true);
	pTile->Get_Component<CRigidbody>()->Set_UseGravity(false);

	// 3. 떨어지는 몬스터
	CMonster* pFallingMonster = CMonster::Create(m_pGraphicDev);

	pFallingMonster->Get_Component<CTransform>()->Set_Pos({ 0.f, 8.f, 10.f }); // 공중, 약간 앞쪽
	pFallingMonster->Get_Component<CRigidbody>()->Set_OnGround(false);
	pFallingMonster->Get_Component<CRigidbody>()->Set_UseGravity(true);
	pFallingMonster->Get_Component<CRigidbody>()->Set_Bounce(0.5f);
	pFallingMonster->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
	pFallingMonster->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);

	// 4. 카메라 (플레이어 시점)
	CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);

	// 5. 플레이어 → 타겟 오브젝트
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"MyPlayer", pPlayer);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"MyTile", pTile);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"FallingMonster", pFallingMonster);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	// 6. 카메라 타겟은 플레이어
	pCam->Set_Target(pPlayer);  // 1인칭 시점
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	return S_OK;
}

_int SceneSB::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);
	CCollisionMgr::Get_Instance()->Update_Collision();

	return 0;
}

void SceneSB::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);

}

SceneSB* SceneSB::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneSB* pScene = new SceneSB(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneSB Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneSB::Free()
{
	CScene::Free();
}