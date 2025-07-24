#pragma once
#include "pch.h"
#include "SceneStage3.h"
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
#include "CPickSwitch.h"
#include "CSlotCube_Auto.h"
#include "CPlayerTriggerCube.h"
#include "CMirrorSlotCube.h"
#include "CZoneSensor.h"

#include "SceneSB.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"

SceneStage3::SceneStage3(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

SceneStage3::~SceneStage3()
{
}


HRESULT SceneStage3::Ready_Scene()
{
	//CSoundMgr::Get_Instance()->Load_Sound("BGM1", "../Bin/Resource/Sound/BGM1.mp3");
	//CSoundMgr::Get_Instance()->Set_Volume("BGM1", 0.5f);
	//CSoundMgr::Get_Instance()->Play("BGM1", "SFX", true);

	Init_Layers();
	for (auto& tile : Get_Layer(LAYER_TILE)->Get_ObjVec()) {
		tile.pObj->Get_Component<CRigidBody>()->Set_UseGravity(false);
	}

	CMainPlayer* pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"MainPlayer");
	pPlayer->Get_Component<CRigidBody>()->Set_UseGravity(true);
	pPlayer->Get_Component<CRigidBody>()->Set_OnGround(true);
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);
	pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 10.f, 120.f });

	CCrosshairUIObject* cross = CCrosshairUIObject::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", cross);
	CUiMgr::Get_Instance()->AddUI(cross);
	pPlayer->Set_Crosshair(cross);

	FFCam* pCam = FFCam::Create(m_pGraphicDev);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	pCam->Set_Target(pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	FloatingSet();
	DirectionSet();
	SlotSet();
	StairSet();

	return S_OK;
}

_int SceneStage3::Update_Scene(const _float& fTimeDelta)
{

	if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CSceneGate>(L"CSceneGate")->Get_InGate()) {
		CScene* pScene = SceneSB::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
		CCollisionMgr::Get_Instance()->Clear();
		CRenderMgr::Get_Instance()->Clear();
	}
	else {
		Set_Triggers();
		CScene::Update_Scene(fTimeDelta);
	}

	return 0;
}

void SceneStage3::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void SceneStage3::FloatingSet()
{
	for (int i = 1; i < 9; i++) {
		wstring name = L"CDoor1_" + to_wstring(i);
		CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(name);
		if (i == 1 || i == 3 || i == 5 || i == 7)
		{
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { -1.f, 0.f, 0.f }, 5.f, 5.f, 0.5f);
		}
		else
		{
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 1.f, 0.f, 0.f }, 5.f, 5.f, 0.5f);
		}
		cube->Set_Loop();
		cube->SetTrigger(true);
	}

	for (int i = 1; i < 8; i++) {
		wstring name = L"CFloatingCube1_1_" + to_wstring(i);
		CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(name);
		cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 1.f, 0.f, 0.f }, 5.f, 5.f, 0.5f);
		cube->Set_Loop();
		cube->SetTrigger(true);
	}
	for (int i = 1; i < 8; i++) {
		wstring name = L"CFloatingCube1_2_" + to_wstring(i);
		CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(name);
		cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { -1.f, 0.f, 0.f }, 5.f, 5.f, 0.5f);
		cube->Set_Loop();
		cube->SetTrigger(true);
	}

	CFloatingCube* CFloatingCube1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1");
	CFloatingCube1_1->Set_Info(CFloatingCube1_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, -1.f, 0.f }, 5.f, 5.f, 0.5f);
	CFloatingCube* CDoor2_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor2_1");
	CDoor2_1->Set_Info(CDoor2_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, -1.f, 0.f }, 12.f, 5.f, 0.5f);

	for (int i = 1; i < 13; i++) {
		wstring name = L"CFloatingCube2_" + to_wstring(i);
		CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(name);
		cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 25.f, 5.f, 0.5f);
	}

	CFloatingCube* CFloatingCube2_1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_1_1");
	CFloatingCube2_1_1->Set_Info(CFloatingCube2_1_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, -1.f, 0.f }, 27.f, 3.f, 0.5f);
	CFloatingCube2_1_1->Set_Loop();
	CFloatingCube2_1_1->SetTrigger(true);


	CFloatingCube* CDoor3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor3_1");
	CDoor3_1->Set_Info(CDoor3_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, -1.f, 0.f }, 30.f, 3.f, 0.5f);

}

void SceneStage3::DirectionSet()
{
	CDirectionalCube* CDCube3_1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_1_1");
	CDCube3_1_1->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_1_1->Set_Info({ 0.f, 0.f, 1.f }, 0.f, 8.f);

	CDirectionalCube* CDCube3_1_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_1_2");
	CDCube3_1_2->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_1_2->Set_Info({ 0.f, 0.f, 1.f }, -4.f, 2.f);

	CDirectionalCube* CDCube3_1_3 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_1_3");
	CDCube3_1_3->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_1_3->Set_Info({ 0.f, 0.f, 1.f }, -4.f, 4.f);
	
	CDirectionalCube* CDCube3_1_4 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_1_4");
	CDCube3_1_4->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_1_4->Set_Info({ 0.f, 0.f, 1.f }, 0.f, 12.f);
	
	CDirectionalCube* CDCube3_1_5 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_1_5");
	CDCube3_1_5->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_1_5->Set_Info({ 0.f, 0.f, 1.f }, -6.f, 0.f);
	
	CDirectionalCube* CDCube3_1_6 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_1_6");
	CDCube3_1_6->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_1_6->Set_Info({ 0.f, 0.f, 1.f }, -2.f, 6.f);

	CDirectionalCube* CDCube3_2_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_2_1");
	CDCube3_2_1->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_2_1->Set_Info({ 1.f, 0.f, 0.f }, -6.f, 2.f);

	CDirectionalCube* CDCube3_2_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_2_2");
	CDCube3_2_2->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_2_2->Set_Info({ 1.f, 0.f, 0.f }, -2.f, 6.f);

	CDirectionalCube* CDCube3_2_3 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube3_2_3");
	CDCube3_2_3->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	CDCube3_2_3->Set_Info({ 1.f, 0.f, 0.f }, -4.f, 4.f);


}

void SceneStage3::SlotSet()
{
	auto pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"MainPlayer");

	for (int i = 1; i < 5; i++) {
		wstring name = L"CSlotSensor1_" + to_wstring(i);
		CSlotSensor* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(name);
		cube->Set_Info(pPlayer, i, i);
		cube->Set_PlayerPick(false);
	}

	for (int i = 1; i < 4; i++) {
		wstring name = L"CSlotCube_Auto1_" + to_wstring(i);
		CSlotCube_Auto* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube_Auto>(name);
		cube->Set_Info(i, i);
	}

	CMirrorSlotCube* CMirrorSlotCube1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CMirrorSlotCube>(L"CMirrorSlotCube1_1");
	CMirrorSlotCube1_1->Set_Info(4, 4);
	CMirrorSlotCube1_1->Set_Player(pPlayer);
	CMirrorSlotCube1_1->Set_Follow(false);
	CMirrorSlotCube1_1->Set_MirrorPlane(_vec3(1.f, 0.f, 0.f), _vec3(1.f, 0.f, 0.f));

	CSlotCube* CSlotCube2_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube2_1");
	CSlotCube2_1->Set_Info(pPlayer, 5, 5);

	CSlotSensor* CSlotSensor2_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor2_1");
	CSlotSensor2_1->Set_Info(pPlayer, 5, 5);

	CSlotSensor* CSlotSensor3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor3_1");
	CSlotSensor3_1->Set_Info(pPlayer, 5, 5);
	CSlotSensor3_1->Set_PlayerPick(false);

	CSlotCube_Auto* CSlotCube_Auto3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube_Auto>(L"CSlotCube_Auto3_1");
	CSlotCube_Auto3_1->Set_Info(5, 5);
}

void SceneStage3::StairSet()
{
	CStairBlock* CStairBlock1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CStairBlock>(L"CStairBlock1_1");
	CStairBlock1_1->Set_Distance(-32.f);
}

void SceneStage3::Set_Triggers()
{
	// 첫번째 문
	_bool bDoorTrigger1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CDoorTrigger1_1")->Get_SensorState();

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_1")->Set_GoBack(!bDoorTrigger1_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_2")->Set_GoBack(!bDoorTrigger1_1);

	// 두번째 문 
	_bool bDoorTrigger1_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CDoorTrigger1_2")->Get_SensorState();

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_3")->Set_GoBack(!bDoorTrigger1_2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_4")->Set_GoBack(!bDoorTrigger1_2);

	///-----첫번째 방
	
	// 블럭 슬롯 퍼즐
	if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor1_1")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor1_2")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor1_3")->Get_SensorState())
	{
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1")->SetTrigger(true);
	}
	
	// 파란 버튼
	_bool CPickSwitch1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch1_1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_1")->Set_GoBack(!CPickSwitch1_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_2")->Set_GoBack(!CPickSwitch1_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_3")->Set_GoBack(!CPickSwitch1_1);
	
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_1")->Set_GoBack(!CPickSwitch1_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_2")->Set_GoBack(!CPickSwitch1_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_3")->Set_GoBack(!CPickSwitch1_1);
	
	// 빨간 버튼
	_bool CPickSwitch1_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch1_2")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_4")->Set_GoBack(!CPickSwitch1_2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_5")->Set_GoBack(!CPickSwitch1_2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_6")->Set_GoBack(!CPickSwitch1_2);
	
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_4")->Set_GoBack(!CPickSwitch1_2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_5")->Set_GoBack(!CPickSwitch1_2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_6")->Set_GoBack(!CPickSwitch1_2);
	
	// 분홍 버튼
	_bool CPickSwitch1_3 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch1_3")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_1_7")->Set_GoBack(!CPickSwitch1_3);
	
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube1_2_7")->Set_GoBack(!CPickSwitch1_3);

	//플레이어 미러 큐브 등록

	_bool bSlotSensor1_4 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor1_4")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor2_1")->SetTrigger(bSlotSensor1_4);
	//Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor2_1")->SetTrigger(true);

	_bool bMirrorTriggerCube1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPlayerTriggerCube>(L"CMirrorTriggerCube1_1")->Get_InGate();
	CMirrorSlotCube* CMirrorSlotCube1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CMirrorSlotCube>(L"CMirrorSlotCube1_1");
	if (bMirrorTriggerCube1_1) {
		if (!bSlotSensor1_4)
		{
			CMirrorSlotCube1_1->Set_Follow(true);
		}
		else {
			CMirrorSlotCube1_1->Set_Follow(false);
		}
	}
	else {
		CMirrorSlotCube1_1->Set_Follow(false);
	}



	// 3개 가림막 풀기
	// 1번째 가림막
	_bool CPickSwitch2_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch2_1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_9")->SetTrigger(CPickSwitch2_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_10")->SetTrigger(CPickSwitch2_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_11")->SetTrigger(CPickSwitch2_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_12")->SetTrigger(CPickSwitch2_1);



	//2번째 퀘스트방
	//입구 문
	_bool bDoorTrigger1_3 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CDoorTrigger1_3")->Get_SensorState();

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_5")->Set_GoBack(!bDoorTrigger1_3);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_6")->Set_GoBack(!bDoorTrigger1_3);

	//첫번째 슬롯
	_bool bSlotSensor3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor3_1")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor3_1")->SetTrigger(bSlotSensor3_1);

	//임시
	//2번째 가림막
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_5")->SetTrigger(bSlotSensor3_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_6")->SetTrigger(bSlotSensor3_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_7")->SetTrigger(bSlotSensor3_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_8")->SetTrigger(bSlotSensor3_1);

	//3번째 퀘스트방
	//입구 문
	_bool bDoorTrigger1_4 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CDoorTrigger1_4")->Get_SensorState();

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_7")->Set_GoBack(!bDoorTrigger1_4);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoor1_8")->Set_GoBack(!bDoorTrigger1_4);

	// 클리어 스위치
	_bool bPickSwitch4_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch4_1")->Get_SwitchState();
	//3번째 가림막
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_1")->SetTrigger(bPickSwitch4_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_2")->SetTrigger(bPickSwitch4_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_3")->SetTrigger(bPickSwitch4_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_4")->SetTrigger(bPickSwitch4_1);


	_bool bSlotSensor2_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor2_1")->Get_SensorState();
	_bool bZoneSensor5_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneSensor5_1")->Get_SensorState();
	_bool bLast = false;

	if (bSlotSensor2_1)
	{
		if (bZoneSensor5_1) {
			bLast = false;
		}
		else {
			bLast = true;
		}
	}
	else {
		bLast = false;
	}

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube2_1_1")->Set_GoBack(!bLast);
	
	

}

SceneStage3* SceneStage3::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneStage3* pScene = new SceneStage3(pGraphicDev);

	return pScene;
}

void SceneStage3::Free()
{
	//CSoundMgr::Get_Instance()->Stop("BGM1");
	CScene::Free();
}