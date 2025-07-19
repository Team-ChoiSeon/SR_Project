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
#include "CPlayer.h"
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
	m_pFFCam->Get_Component<CCamera>()->Set_Far(1000.f);

	//UI Setting
	CUiMgr::Get_Instance()->Ready_UiMgr();
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));
	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	m_pPlayer->Set_Crosshair(Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair"));


	//Scene Setting
	FloatingSet();
	DirectionSet();
	SlotSet();
	MagnetSet();


	//temp
	auto gravitycube = CLowGravityCube::Create(m_pGraphicDev);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"gravitycube", gravitycube);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"gravitycube")->Get_Component<CTransform>()->Set_Pos({ 0.f, 21.f, -5.f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"gravitycube")->Get_Component<CTransform>()->Set_Scale({1.f, 0.2f, 1.f });

	auto zonesensor = CZoneSensor::Create(m_pGraphicDev);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"zonesensor", zonesensor);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"zonesensor")->Get_Component<CTransform>()->Set_Pos({ 10.f, 21.f, -5.f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"zonesensor")->Get_Component<CTransform>()->Set_Scale({ 1.f, 0.2f, 1.f });


	return S_OK;
}

TestSceneHW* TestSceneHW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	TestSceneHW* pScene = new TestSceneHW(pGraphicDev);

	return pScene;
}

int TestSceneHW::Update_Scene(const _float& fTimeDelta)
{
	//m_pFloatingCube->SetTrigger(Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_1")->Get_SwitchState());

	if (m_bSceneFirst) //처음 시작시 업데이트를 지나치고 ftimedelta를 한번 무시하게 하여 오브젝트들이 중력을 과하게 받아서 떨어지는 현상 방지
	{
		m_bSceneFirst = false;
		return S_OK;
	}


	_bool Floating1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_1")->SetTrigger(Floating1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_2")->SetTrigger(Floating1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_3")->SetTrigger(Floating1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_13")->SetTrigger(Floating1);

	_bool Floating2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_2")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->SetTrigger(Floating2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->SetTrigger(Floating2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_6")->SetTrigger(Floating2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_7")->SetTrigger(Floating2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_8")->SetTrigger(Floating2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_9")->SetTrigger(Floating2);

	_bool Floating3 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_3")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_10")->SetTrigger(Floating3);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_11")->SetTrigger(Floating3);

	_bool bPuzzle1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_1")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_2")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_3")->Get_SensorState();

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_12")->SetTrigger(bPuzzle1);

	CScene::Update_Scene(fTimeDelta);



	//===========================================================================================================//
	//Debugging Codes

	//CGuiSystem::Get_Instance()->RegisterPanel("velocity", [this]() {
	//	// 간단한 GUI 창 하나 출력
	//	ImGui::SetNextWindowSize(ImVec2{ 200,200 });
	//	ImGui::Begin("IMGUI");
	//	ImGui::Text(to_string(m_pPlayer->Get_Component<CTransform>()->Get_Pos().y).c_str());
	//	ImGui::End();
	//	});

	auto state = Get_Layer(LAYER_OBJECT)->Get_GameObject<CMetalCube>(L"CMetalCube_1")->m_eState;
	_bool zonesensor = Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"zonesensor")->Get_SensorState();
	//Deubbing Code
	 CGuiSystem::Get_Instance()->RegisterPanel("state", [zonesensor]() {
 		// 간단한 GUI 창 하나 출력
 		ImGui::SetNextWindowSize(ImVec2{ 200,200 });
		if (zonesensor)
			ImGui::Begin("TRUE");
		else
			ImGui::Begin("FALSE");
		 //switch (state)
		 //{
		 //case METAL_STATE::IDLE:
			// ImGui::Begin("IDLE");
			// break;
		 //case METAL_STATE::APPROACH:
			// ImGui::Begin("APPROACH");
			// break;
		 //case METAL_STATE::SYNC:
			// ImGui::Begin("SYNC");
			// break;
		 //case METAL_STATE::DETACH:
			// ImGui::Begin("DETACH");
		 //}
		 //if (m_pRigid->Get_OnGround())
		 //    ImGui::Begin("On Ground");
		 //else if (!m_pRigid->Get_OnGround())
		 //    ImGui::Begin("Not On Ground");

 		ImGui::End();

 		});
	//_vec3 playerpos = m_pPlayer->GetPos();

	//wchar_t buf1[128];
	//swprintf_s(buf1, 128, L"Player Pos : %.3f, %.3f, %.3f\n", playerpos.x, playerpos.y, playerpos.z);
	//OutputDebugStringW(buf1);
	//_vec3 slotpos = m_pSlotCube->Get_Component<CTransform>()->Get_Pos();
	//_bool slotground = m_pSlotCube->Get_Component<CRigidBody>()->Get_OnGround();

	//wchar_t buf1[128];
	//swprintf_s(buf1, 128, L"Cube Pos : %.3f, %.3f, %.3f\n", slotpos.x, slotpos.y, slotpos.z);
	//OutputDebugStringW(buf1);

	//if(slotground)
	//	OutputDebugStringW(L"Cube On Ground\n");
	//else
	//	OutputDebugStringW(L"Cube Not On Ground\n");
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
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_1")->Set_Info( { 1, 0, 0 }, 5, 3, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_2")->Set_Info( { 1, 0, 0 }, 5, 3, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_3")->Set_Info( { 1, 0, 0 }, 5, 3, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_13")->Set_Info( { 1, 0, 0 }, 5, 3, 0);

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->Set_Info( { 0, 1, 0 }, 20.6f, 10, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->Set_Info( { 0, 1, 0 }, 20.6f, 9, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_6")->Set_Info( { 0, 1, 0 }, 20.6f, 8, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_7")->Set_Info( { 0, 1, 0 }, 20.6f, 7, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_8")->Set_Info( { 0, 1, 0 }, 20.6f, 6, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_9")->Set_Info( { 0, 1, 0 }, 20.6f, 5, 0);
	
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->Get_Component<CTransform>()->Set_Scale({ 4.f, 0.4f, 3.98f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->Get_Component<CTransform>()->Set_Scale({ 4.f, 0.4f, 3.98f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_6")->Get_Component<CTransform>()->Set_Scale({ 4.f, 0.4f, 3.98f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_7")->Get_Component<CTransform>()->Set_Scale({ 4.f, 0.4f, 3.98f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_8")->Get_Component<CTransform>()->Set_Scale({ 4.f, 0.4f, 3.98f });
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_9")->Get_Component<CTransform>()->Set_Scale({ 4.f, 0.4f, 3.98f });

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_10")->Set_Info( { 0, 1, 0 }, 1, 0.7f, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_11")->Set_Info( {0, 1, 0}, 40, 10, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_12")->Set_Info( { 0, 0.88f, 1.f }, 61, 5, 2);

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_11")->Set_Loop();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_12")->Set_Loop();
}

void TestSceneHW::DirectionSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_1")->Set_Info( { 0, 1, 0 }, 0.f, 5.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_2")->Set_Info( { 0, 1, 0 }, 0.f, 5.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_3")->Set_Info( { 0, 1, 0 }, 0.f, 5.f);

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_4")->Set_Info( { -1, 0, 0 }, 0.f, 20.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_5")->Set_Info( { 1, 0, 0 }, 0.f, 20.f);
}

void TestSceneHW::SlotSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_1")->Set_Info(m_pPlayer, 1, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_2")->Set_Info(m_pPlayer, 1, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_3")->Set_Info(m_pPlayer, 1, 3);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Set_Info( 1, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Set_Info( 1, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Set_Info( 1, 3);
}

void TestSceneHW::MagnetSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMagneticCube>(L"CMagneticCube_1")->Get_Component<CCollider>()->Set_BoundType(BoundingType::OBB);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMagneticCube>(L"CMagneticCube_2")->Get_Component<CCollider>()->Set_BoundType(BoundingType::OBB);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMagneticCube>(L"CMagneticCube_3")->Get_Component<CCollider>()->Set_BoundType(BoundingType::OBB);

	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_1")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_2")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_3")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_4")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_5")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_6")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_7")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_8")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_9")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_10")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_11")->Set_Info(m_pPlayer);
	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_12")->Set_Info(m_pPlayer);
}

