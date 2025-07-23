#pragma once
#include "pch.h"
#include "SceneStage1.h"
#include "CMainPlayer.h"
#include "CFirstviewFollowingCamera.h"
#include "DummyCube.h"

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

#include "SceneStage2.h"
#include "CRenderMgr.h"

SceneStage1::SceneStage1(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

SceneStage1::~SceneStage1()
{
}

HRESULT SceneStage1::Ready_Scene()
{
	Init_Layers();

	//Player Setting
	m_pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1");
	CSceneMgr::Get_Instance()->Set_Player(m_pPlayer);

	//Camera Setting
	m_pFFCam = Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"CFirstviewFollowingCamera_1");
	m_pFFCam->Set_Target(m_pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);
	m_pFFCam->Get_Component<CCamera>()->Set_Far(1000.f);

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
	SensorSet();
	SoundSet();

	CSoundMgr::Get_Instance()->Mute_Group("SFX", true);
	m_bSfxMuted = true;
	m_fMuteTimer = 2.f;


	return S_OK;
}

SceneStage1* SceneStage1::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneStage1* pScene = new SceneStage1(pGraphicDev);

	return pScene;
}

int SceneStage1::Update_Scene(const _float& fTimeDelta)
{


	//Room4
	if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CSceneGate>(L"Room4_SceneGate")->Get_InGate()) {
		CScene* pScene = SceneStage2::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
		CSoundMgr::Get_Instance()->Stop_Group("BGM");
		CSoundMgr::Get_Instance()->Stop_Group("SFX");
		CSoundMgr::Get_Instance()->Stop_Group("ENV");
		CCollisionMgr::Get_Instance()->Clear();
		CRenderMgr::Get_Instance()->Clear();
	}
	else {
		SoundUpdate(fTimeDelta);
		Room1Update(fTimeDelta);
		Path1Update(fTimeDelta);
		Room2Update(fTimeDelta);
		Room3Update(fTimeDelta);
		Path2Update(fTimeDelta);
		Room4Update(fTimeDelta);
		Room5Update(fTimeDelta);

		CScene::Update_Scene(fTimeDelta);

	}



	//===========================================================================================================//
	//Debugging Codes

	//auto zs = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor");
	//auto playerpos = m_pPlayer->Get_Component<CTransform>()->Get_Pos();
	//auto aabb = zs->Get_Component<CCollider>()->Get_AABBW();
	//bool on = zs->Get_SensorState();

	//CGuiSystem::Get_Instance()->RegisterPanel("Z3_Debug", [=]() {

	//	ImGui::Text("PlayerPos: (%.1f, %.1f, %.1f)",
	//		playerpos.x, playerpos.y, playerpos.z);
	//	ImGui::Text("Room3 AABB Min: (%.1f, %.1f, %.1f)",
	//		aabb.vMin.x, aabb.vMin.y, aabb.vMin.z);
	//	ImGui::Text("Room3 AABB Max: (%.1f, %.1f, %.1f)",
	//		aabb.vMax.x, aabb.vMax.y, aabb.vMax.z);
	//	ImGui::Text("SensorOn: %s", on ? "TRUE" : "FALSE");
	//	});

	//_bool switchon = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Room3_Switch1")->Get_SwitchState();
	// CGuiSystem::Get_Instance()->RegisterPanel("state", [switchon]() {
	//		// 간단한 GUI 창 하나 출력
	//		ImGui::SetNextWindowSize(ImVec2{ 200,200 });
	//	if (switchon)
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

 		//ImGui::End();
  	//	});
	//===========================================================================================================//


	return 0;
}

void SceneStage1::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);

}



void SceneStage1::Free()
{
	Clear_Layers();

	CScene::Free();
}

void SceneStage1::FloatingSet()
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
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room4_SlidingDoor1")->Set_Info({ 0, 1, 0 }, 15, 20, 0);

	}

	//Elevator
	{
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_Floating1")->Set_Info({ 1, 0, 0 }, 10, 4, 0);

		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->Set_Info({ 0, 1, 0 }, 20, 5, 1.5f);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->Set_Loop();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->Set_Info({ 0, 1, 0 }, 20, 5, 1.5f);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->Set_Loop();

		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->Set_Info({ 0, 1, 0 }, 20, 5, 1.5f);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->Set_Loop();
	}
}

void SceneStage1::DirectionSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_DirectionalDoor")->Set_Info({ 0, 0, -1 }, 10);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Directional1")->Set_Info({ 1, 0, 0 }, 3.8f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Directional2")->Set_Info({ 1, 0, 0 }, 3.8f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Direcitonal3")->Set_Info({ 1, 0, 0 }, 3.8f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"Room4_Driectional4")->Set_Info({ 1, 0, 0 }, 3.8f);
}

void SceneStage1::SlotSet()
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

void SceneStage1::MagnetSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"Room4_MetalCube1")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"Room4_MetalCube2")->Set_Info(m_pPlayer);
}

void SceneStage1::SensorSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room1_DoorSensor1")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path1_DoorSensor")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room2_ZoneSensor")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_FloatingSensor1")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_DoorSensor")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room5_ZoneSensor1")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room5_ZoneSensor2")->Set_DetectTarget(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor")->Set_DetectTarget(m_pPlayer);

}

void SceneStage1::SoundSet()
{
	//Sound Setting

	CSoundMgr::Get_Instance()->Load_Sound("Collision1", "../Bin/Resource/Sound/Cube/MetalCollision_1.mp3");
	CSoundMgr::Get_Instance()->Load_Sound("Collision2", "../Bin/Resource/Sound/Cube/MetalCollision_2.mp3");
	CSoundMgr::Get_Instance()->Load_Sound("Collision3", "../Bin/Resource/Sound/Cube/MetalCollision_3.ogg");
	CSoundMgr::Get_Instance()->Load_Sound("Collision4", "../Bin/Resource/Sound/Cube/MetalCollision_7.wav");
	CSoundMgr::Get_Instance()->Load_Sound("Collision5", "../Bin/Resource/Sound/Cube/MetalCollision_8.wav");

	CSoundMgr::Get_Instance()->Load_Sound("MagnetField", "../Bin/Resource/Sound/Cube/Magnet_1.wav");

	CSoundMgr::Get_Instance()->Load_Sound("DoorOpen1", "../Bin/Resource/Sound/Cube/DoorOpen_2.wav");
	CSoundMgr::Get_Instance()->Load_Sound("DoorOpen2", "../Bin/Resource/Sound/Cube/DoorOpen_4.wav");
	CSoundMgr::Get_Instance()->Load_Sound("DoorClose1", "../Bin/Resource/Sound/Cube/DoorClose_1.wav");
	
	CSoundMgr::Get_Instance()->Load_Sound("CubeDrag", "../Bin/Resource/Sound/Cube/MetalDrag_5.wav");
	CSoundMgr::Get_Instance()->Load_Sound("ElevatorStart", "../Bin/Resource/Sound/Cube/ElevatorStart_1.wav");
	CSoundMgr::Get_Instance()->Load_Sound("ElevatorMove", "../Bin/Resource/Sound/Cube/ElevatorMove_2.wav");
	CSoundMgr::Get_Instance()->Set_Volume("ElevatorMove", 0.5f);

	CSoundMgr::Get_Instance()->Load_Sound("Correct", "../Bin/Resource/Sound/Puzzle/Correct_3.wav");
	CSoundMgr::Get_Instance()->Load_Sound("Wrong", "../Bin/Resource/Sound/Puzzle/Wrong_2.mp3");
	CSoundMgr::Get_Instance()->Load_Sound("Switch", "../Bin/Resource/Sound/Switch/Pick_1.mp3");


	CSoundMgr::Get_Instance()->Load_Sound("BGM", "../Bin/Resource/Sound/BGM1.mp3");
	CSoundMgr::Get_Instance()->Load_Sound("SpaceHowl", "../Bin/Resource/Sound/BackGround/Space_3.wav");
	CSoundMgr::Get_Instance()->Load_Sound("Spaceship", "../Bin/Resource/Sound/BackGround/Spaceship_1.wav");
	CSoundMgr::Get_Instance()->Set_Volume("BGM", 0.5f);
	CSoundMgr::Get_Instance()->Set_Volume("SpaceHowl", 0.3f);
	CSoundMgr::Get_Instance()->Set_Volume("Spaceship", 0.3f);
	CSoundMgr::Get_Instance()->Play("BGM", "BGM", true);
	CSoundMgr::Get_Instance()->Play("SpaceHowl", "BGM", true);
	CSoundMgr::Get_Instance()->Play("Spaceship", "BGM", true);


}

void SceneStage1::SoundUpdate(const _float& fTimeDelta)
{
	if (m_bSfxMuted) {
		m_fMuteTimer -= fTimeDelta;
		if (m_fMuteTimer <= 0.0f) {
			CSoundMgr::Get_Instance()->Mute_Group("SFX", false);
			m_bSfxMuted = false;
		}
	}
}

void SceneStage1::Room1Update(const _float& fTimeDelta)
{
	////Room1
	//Door
	_bool door1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room1_DoorSensor1")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room1_SlidingDoor1")->SetGoBack(!door1);

	//PlaySound
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room1_SlidingDoor1")->PlayDoorSound(Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room1_DoorSensor1")->Get_OnEdge(),
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room1_DoorSensor1")->Get_OffEdge());
}

void SceneStage1::Path1Update(const _float& fTimeDelta)
{
	// Path1
	_bool door2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Path1_DoorSwitch1")->Get_SwitchState() &&
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path1_DoorSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path1_SlidingDoor1")->SetGoBack(!door2);

	//PlaySound
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path1_SlidingDoor1")->PlayDoorSound
	(Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Path1_DoorSwitch1")->Get_OnEdge() && door2,
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path1_DoorSensor")->Get_OffEdge());
}

void SceneStage1::Room2Update(const _float& fTimeDelta)
{
	////Room2
	//Switch
	_bool room2Switch1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Room2_Switch1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_Floating1")->SetTrigger(room2Switch1);

	//Slot
	_bool room2Slotsensor1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room2_SlotSensor1")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room2_ZoneSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_SlidingDoor1")->SetGoBack(!room2Slotsensor1);

	//PlaySound
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_SlidingDoor1")->
		PlayDoorSound(Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room2_SlotSensor1")->Get_OnEdge() && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room2_ZoneSensor")->Get_SensorState(),
			Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room2_ZoneSensor")->Get_OffEdge());

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room2_Floating1")->PlayElevatorSound();
}

void SceneStage1::Room3Update(const _float& fTimeDelta)
{	
	////Room3
	//Switch
	_bool room3Switch1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"Room3_Switch1")->Get_SwitchState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->SetTrigger(room3Switch1);

	//Slot1
	_bool room3SlotPuzzle1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor1")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor2")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->SetTrigger(room3SlotPuzzle1);

	//Slot2
	_bool room3SlotPuzzle2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor3")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_SlidingDoor1")->SetGoBack(!room3SlotPuzzle2);

	//PlaySound
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_SlidingDoor1")->PlayDoorSound
	(Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"Room3_SlotSensor3")->Get_OnEdge(),
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor")->Get_OffEdge());

	//if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Room3_ZoneSensor")->Get_SensorState())
	{
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating1")->PlayElevatorSound();
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Room3_Floating2")->PlayElevatorSound();
	}


}

void SceneStage1::Path2Update(const _float& fTimeDelta)
{	
	////Path2
	_bool path2Elevtor1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_FloatingSensor1")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->SetTrigger(path2Elevtor1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_Floating1")->PlayElevatorSound();

	_bool path2Door1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_DoorSensor")->Get_SensorState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_SlidingDoor1")->SetGoBack(!path2Door1);

	//PlaySound
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Path2_SlidingDoor1")->PlayDoorSound(
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_DoorSensor")->Get_OnEdge(),
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"Path2_DoorSensor")->Get_OffEdge());
}

void SceneStage1::Room4Update(const _float& fTimeDelta)
{
}

void SceneStage1::Room5Update(const _float& fTimeDelta)
{

}

