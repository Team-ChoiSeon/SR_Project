#pragma once
#include "pch.h"
#include "BossScene.h"


#include "CMainPlayer.h"
#include "CMonster.h"
#include "CCameraMgr.h"
#include "CFirstviewFollowingCamera.h"
#include "CTestTile.h"
#include "CVellum.h"
#include "CImpulseCube.h"

#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CUiMgr.h"
#include "CResourceMgr.h"


BossScene::BossScene(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

BossScene::~BossScene()
{
}

HRESULT BossScene::Ready_Scene()
{

	//CScene::Ready_Scene();
	Init_Layers();

	// 1. 플레이어 (시점 고정)
	CMainPlayer* pPlayer = CMainPlayer::Create(m_pGraphicDev);
	pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 30.f, -75.f });
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);

	CCrosshairUIObject* cross = CCrosshairUIObject::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", cross);
	CUiMgr::Get_Instance()->AddUI(cross);
	pPlayer->Set_Crosshair(cross);

	// 3-2. 벨룸
	CVellum* pVellum = CVellum::Create(m_pGraphicDev);

	// 4. 카메라 (플레이어 시점)
	CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);

	// 5. 플레이어 → 타겟 오브젝트
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", pPlayer);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"Vellum", pVellum);



	// 6. 카메라 타겟은 플레이어
	pCam->Set_Target(pPlayer);  // 1인칭 시점
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	CResourceMgr::Get_Instance()->Load_Texture(L"vecteezy_smoke-effect-transparent_21104616.png");
	CResourceMgr::Get_Instance()->Load_Texture(L"blackSmoke00.png");
	CResourceMgr::Get_Instance()->Load_Texture(L"projectile.png");

	CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev, L"Head_Smile.obj");
	CResourceMgr::Get_Instance()->Load_Material(L"Head_Smile.mtl");
	CResourceMgr::Get_Instance()->Load_Texture(L"Head_Smile.png");

	CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev, L"Head_Fire.obj");
	CResourceMgr::Get_Instance()->Load_Material(L"Head_Fire.mtl");
	CResourceMgr::Get_Instance()->Load_Texture(L"Head_Fire.png");

	CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev, L"Head_Dead.obj");
	CResourceMgr::Get_Instance()->Load_Material(L"Head_Dead.mtl");
	CResourceMgr::Get_Instance()->Load_Texture(L"Head_Dead.png");

	CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev, L"Head_Hit.obj");
	CResourceMgr::Get_Instance()->Load_Material(L"Head_Hit.mtl");
	CResourceMgr::Get_Instance()->Load_Texture(L"Head_Hit.png");

	CResourceMgr::Get_Instance()->Load_Mesh(m_pGraphicDev, L"Head_Sleep.obj");
	CResourceMgr::Get_Instance()->Load_Material(L"Head_Sleep.mtl");
	CResourceMgr::Get_Instance()->Load_Texture(L"Head_Sleep.png");

	return S_OK;
}

_int BossScene::Update_Scene(const _float& fTimeDelta)
{
	CScene::Update_Scene(fTimeDelta);
	return 0;
}

void BossScene::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);

}

BossScene* BossScene::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	BossScene* pScene = new BossScene(pGraphicDev);
	return pScene;
}

void BossScene::Free()
{
	CScene::Free();
}