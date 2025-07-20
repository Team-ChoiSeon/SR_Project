#pragma once
#include "pch.h"
#include "SceneHW.h"
#include "CMainPlayer.h"
#include "CFirstviewFollowingCamera.h"
#include "DummyCube.h"
#include "CCamera.h"
#include "CTimeMgr.h"
#include "CCameraMgr.h"
#include "CInputMgr.h"
#include "CTransform.h"
#include "CPickingMgr.h"
#include "CFloatingCube.h"
#include "CDirectionalCube.h"
#include "CImpulseCube.h"
#include "CMonster.h"
#include "CTestTile.h"
#include "CCollisionMgr.h"

#include "CRigidBody.h"
#include "CCrosshairUIObject.h"
#include "CUiMgr.h"
#include "TestSceneHW.h"
#include "CFactory.h"
#include "CMesh.h"
#include "CSceneMgr.h"
#include "CSlotSensor.h"
#include "CSlotCube.h"
#include "CPickSwitch.h"
#include "Engine_GUI.h"
#include "CGuiSystem.h"
#include "CMagneticCube.h"
#include "CMetalCube.h"
#include "CSceneGate.h"
#include "CLowGravityCube.h"
#include "CZoneSensor.h"

TestSceneHW::TestSceneHW(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

TestSceneHW::~TestSceneHW()
{
}

HRESULT TestSceneHW::Ready_Scene()
{
	Init_Layers();

	//Player Setting
	m_pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1");
	CSceneMgr::Get_Instance()->Set_Player(m_pPlayer);

	//Camera Setting
	m_pFFCam = Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"CFirstviewFollowingCamera_1");
	m_pFFCam->Set_Target(m_pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);
	m_pFFCam->Get_Component<CCamera>()->Set_Far(300.f);

	//UI Setting
	CUiMgr::Get_Instance()->Ready_UiMgr();
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));
	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	m_pPlayer->Set_Crosshair(Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair"));

	//Object Setting
	FloatingSet();
	DirectionSet();
	SlotSet();
	MagnetSet();

	return S_OK;
}

TestSceneHW* TestSceneHW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	TestSceneHW* pScene = new TestSceneHW(pGraphicDev);

	return pScene;
}

int TestSceneHW::Update_Scene(const _float& fTimeDelta)
{
	//Room1
	_bool door1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room1_DoorSensor1")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room1_SlidingDoor1")->SetGoBack(!door1);

	//Path1
	_bool door2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Path1_DoorSwitch1")->Get_SwitchState() &&
		!Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room2_ZoneSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path1_SlidingDoor1")->SetGoBack(!door2);

	//Room2
	_bool room2Switch1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Room2_Switch1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_Floating1")->SetTrigger(room2Switch1);
	_bool room2Slotsensor1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room2_SlotSensor1")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room2_ZoneSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_SlidingDoor1")->SetGoBack(!room2Slotsensor1);

	//Room3
	_bool room3Switch1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Room3_Switch1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->SetTrigger(room3Switch1);
	_bool room3SlotPuzzle1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor1")->Get_SensorState() &&
							Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor2")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->SetTrigger(room3SlotPuzzle1);
	_bool room3SlotPuzzle2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor3")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_SlidingDoor1")->SetGoBack(!room3SlotPuzzle2);

	//Path2
	_bool path2Elevtor1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_FloatingSensor1")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->SetTrigger(path2Elevtor1);
	_bool path2Door1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_DoorSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_SlidingDoor1")->SetGoBack(!path2Door1);




	CScene::Update_Scene(fTimeDelta);

	//===========================================================================================================//
	//Debugging Codes

	// CGuiSystem::Get_Instance()->RegisterPanel("state", [zonesensor]() {
	//		// 간단한 GUI 창 하나 출력
	//		ImGui::SetNextWindowSize(ImVec2{ 200,200 });
	//	if (zonesensor)
	//		ImGui::Begin("TRUE");
	//	else
	//		ImGui::Begin("FALSE");
	//	 //switch (state)
	//	 //{
	//	 //case METAL_STATE::IDLE:
	//		// ImGui::Begin("IDLE");
	//		// break;
	//	 //case METAL_STATE::APPROACH:
	//		// ImGui::Begin("APPROACH");
	//		// break;
	//	 //case METAL_STATE::SYNC:
	//		// ImGui::Begin("SYNC");
	//		// break;
	//	 //case METAL_STATE::DETACH:
	//		// ImGui::Begin("DETACH");
	//	 //}
	//	 //if (m_pRigid->Get_OnGround())
	//	 //    ImGui::Begin("On Ground");
	//	 //else if (!m_pRigid->Get_OnGround())
	//	 //    ImGui::Begin("Not On Ground");

 //		ImGui::End();
	//===========================================================================================================//


	return 0;
}

void TestSceneHW::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
	
}



void TestSceneHW::Free()
{
	Clear_Layers();

	CScene::Free();
}

void TestSceneHW::FloatingSet()
{	
	//SlidingDoor
	{
		//Room1
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room1_SlidingDoor1")->Set_Info({ 0, 1, 0 }, 15, 20, 0);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room1_SlidingDoor1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room1_SlidingDoor1")->SetTrigger(true);

		//Path1
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path1_SlidingDoor1")->Set_Info({ 0, 1, 0 }, 15, 20, 0);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path1_SlidingDoor1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path1_SlidingDoor1")->SetTrigger(true);

		//Room2
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_SlidingDoor1")->Set_Info({ 0, 1, 0 }, 15, 20, 0);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_SlidingDoor1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_SlidingDoor1")->SetTrigger(true);

		//Room3
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_SlidingDoor1")->Set_Info({ 0, 1, 0 }, 20, 20, 0);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_SlidingDoor1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_SlidingDoor1")->SetTrigger(true);

		//Path2
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_SlidingDoor1")->Set_Info({ 0, 1, 0 }, 15, 20, 0);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_SlidingDoor1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_SlidingDoor1")->SetTrigger(true);

		//Room4
	}

	//Elevator
	{
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_Floating1")->Set_Info({ 1, 0, 0 }, 5, 3, 0);

		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->Set_Info({ 0, 1, 0 }, 20, 5, 1.5f);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->Set_Info({ 0, 1, 0 }, 20, 5, 1.5f);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->Set_Loop();

		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->Set_Info({ 0, 1, 0 }, 20, 5, 1.5f);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->Set_Loop();
	}
}

void TestSceneHW::DirectionSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_DirectionalDoor")->Set_Info({ 0, 0, -1 }, 10);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Directional1")->Set_Info({ 1, 0, 0 }, 3.8f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Directional2")->Set_Info({ 1, 0, 0 }, 3.8f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Direcitonal3")->Set_Info({ 1, 0, 0 }, 3.8f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Driectional4")->Set_Info({ 1, 0, 0 }, 3.8f);
}

void TestSceneHW::SlotSet()
{
	//Room2
	{
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room2_SlotSensor1")->Set_Info(m_pPlayer, 2, 1);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"Room2_SlotCube1")->Set_Info(2, 1);
	}

	//Room3
	{
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor1")->Set_Info(m_pPlayer, 3, 1);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"Room3_SlotCube1")->Set_Info(3, 1);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor2")->Set_Info(m_pPlayer, 3, 2);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"Room3_SlotCube2")->Set_Info(3, 2);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor3")->Set_Info(m_pPlayer, 3, 3);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"Room3_SlotCube3")->Set_Info(3, 3);
	}
}

void TestSceneHW::MagnetSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"Room4_MetalCube1")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"Room4_MetalCube2")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"Room4_MetalCube3")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"Room4_MetalCube4")->Set_Info(m_pPlayer);
}

