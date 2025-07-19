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

	// 1. �÷��̾� (���� ����)
	CMainPlayer* pPlayer = CMainPlayer::Create(m_pGraphicDev);
	pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 30.f, -75.f });
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);

	// 3-2. ����
	CVellum* pVellum = CVellum::Create(m_pGraphicDev);

	// 4. ī�޶� (�÷��̾� ����)
	CFirstviewFollowingCamera* pCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);

	// 5. �÷��̾� �� Ÿ�� ������Ʈ
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", pPlayer);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"Vellum", pVellum);



	// 6. ī�޶� Ÿ���� �÷��̾�
	pCam->Set_Target(pPlayer);  // 1��Ī ����
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);



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