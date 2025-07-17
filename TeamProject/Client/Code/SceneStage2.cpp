#pragma once
#include "pch.h"
#include "SceneStage2.h"
#include "CFactory.h"

#include "CTimeMgr.h"
#include "CInputMgr.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CResourceMgr.h"
#include "CPickingMgr.h"
#include "CCollisionMgr.h"
#include "CSceneMgr.h"
#include "CRenderMgr.h"

#include "CPlayer.h"
#include "CMainPlayer.h"
#include "CLightObject.h"
#include "CTestLightMeshObject.h"
#include "CCrosshairUIObject.h"
#include "DummyCube.h"
#include "CDirectionalCube.h"
#include "CTestTile.h"
#include "CSlotSensor.h"
#include "CSlotCube.h"
#include "CFloatingCube.h"
#include "CStairBlock.h"
#include "CSceneGate.h"

#include "SceneBG.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"

SceneStage2::SceneStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

SceneStage2::~SceneStage2()
{
}

HRESULT SceneStage2::Ready_Scene()
{
	Init_Layers();

	CMainPlayer* pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"MainPlayer");
	pPlayer->Get_Component<CRigidBody>()->Set_UseGravity(true);
	pPlayer->Get_Component<CRigidBody>()->Set_OnGround(true);
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);

	CCrosshairUIObject* cross = CCrosshairUIObject::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", cross);
	CUiMgr::Get_Instance()->AddUI(cross);
	pPlayer->Set_Crosshair(cross);

	FFCam* pCam = FFCam::Create(m_pGraphicDev);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	pCam->Set_Target(pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	for (int i = 1; i < 6; i++) {
		wstring name = L"CSlotQuest_" + to_wstring(i);
		CSlotSensor* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(name);
		cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
		cube->Get_Component<CRigidBody>()->Set_OnGround(true);
		cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
		cube->Get_Component<CCollider>()->Set_ColType(ColliderType::TRIGGER);
		cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::OBB);
		cube->Set_Info(pPlayer, i, i);
	}

	for (int i = 1; i < 6; i++) {
		wstring name = L"CQuestCube_" + to_wstring(i);
		CSlotCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(name);
		cube->Get_Component<CRigidBody>()->Set_UseGravity(true);
		cube->Get_Component<CRigidBody>()->Set_OnGround(false);
		cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
		cube->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
		cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);
		cube->Set_Info(pPlayer, i, i);
	}

	for (int i = 0; i < 9; i++) {
		wstring name = L"CDcube1_" + to_wstring(i);
		CDirectionalCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(name);
		cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
		cube->Get_Component<CRigidBody>()->Set_OnGround(true);
		cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
		cube->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
		cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);

		if (i == 0)
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 0.f, 15.f);
		else if (i > 4)
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 0.f, -1.f }, 0.f, 5.f);
		else
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 0.f, 1.f }, 0.f, 5.f);
	}

	CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube1_1");
	cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
	cube->Get_Component<CRigidBody>()->Set_OnGround(true);
	cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	cube->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);
	cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 1.f, 0.f, 0.f }, 39.f, 15.f, 0.5f);
	cube->Set_Loop();
	cube->SetTrigger(true);

	CStairBlock* pStaircube1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CStairBlock>(L"CStair2_1");
	pStaircube1->Set_Distance(-30.f);
	CStairBlock* pStaircube2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CStairBlock>(L"CStair2_2");
	pStaircube2->Set_Distance(18.f);

	return S_OK;
}

_int SceneStage2::Update_Scene(const _float& fTimeDelta)
{

	if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CSceneGate>(L"CSceneGate_1")->Get_InGate()) {
		CScene* pScene = SceneBG::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
		CCollisionMgr::Get_Instance()->Clear();
		CRenderMgr::Get_Instance()->Clear();
	}
	else {
		CScene::Update_Scene(fTimeDelta);
	}

	return 0;
}

void SceneStage2::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

SceneStage2* SceneStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneStage2* pScene = new SceneStage2(pGraphicDev);

	return pScene;
}

void SceneStage2::Free()
{
	CScene::Free();
}