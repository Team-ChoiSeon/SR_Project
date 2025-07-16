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
TestSceneHW::TestSceneHW(LPDIRECT3DDEVICE9 pGraphicDev)
	: CScene(pGraphicDev)
{
}

TestSceneHW::~TestSceneHW()
{
}

HRESULT TestSceneHW::Ready_Scene()
{
	CUiMgr::Get_Instance()->Ready_UiMgr();
	Init_Layers();
	CFactory::DeSerializeScene(L"../../Scene/hwtest_2.json", this);

	m_pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1");
	CSceneMgr::Get_Instance()->Set_Player(m_pPlayer);

	m_pFFCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"CFirstviewFollowingCamera_1", m_pFFCam);
	m_pFFCam->Set_Target(m_pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);



	FloatingSet();
	DirectionSet();
	SlotSet();

	Get_Layer(LAYER_OBJECT)->Get_GameObject <CMetalCube>(L"CMetalCube_1")->Set_Info(m_pPlayer);


	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));
	Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1")->Set_Crosshair(Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair"));
	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));

	return S_OK;
}

TestSceneHW* TestSceneHW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	TestSceneHW* pScene = new TestSceneHW(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("TestSceneHW Create Failed");
		return nullptr;
	}

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


	_bool bPuzzle1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_1")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_2")->Get_SensorState();
	_bool bPuzzle2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_3")->Get_SensorState() &&
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_4")->Get_SensorState();

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_1")->SetTrigger(Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_1")->Get_SwitchState());
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_2")->SetTrigger(Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_1")->Get_SwitchState());
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_3")->SetTrigger(Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_1")->Get_SwitchState());
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->SetTrigger(bPuzzle1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->SetTrigger(bPuzzle2);
	CScene::Update_Scene(fTimeDelta);



	//===========================================================================================================//
	//Debugging Codes

	CGuiSystem::Get_Instance()->RegisterPanel("velocity", [this]() {
		// 간단한 GUI 창 하나 출력
		ImGui::SetNextWindowSize(ImVec2{ 200,200 });
		ImGui::Begin("IMGUI");
		ImGui::Text(to_string(m_pPlayer->Get_Component<CTransform>()->Get_Pos().y).c_str());
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

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_1")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_1")->Get_Component<CTransform>()->Get_Pos(), { 0, 0, -1 }, 4, 1, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_2")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_2")->Get_Component<CTransform>()->Get_Pos(), { 0, 0, -1 }, 4, 1, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_3")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_3")->Get_Component<CTransform>()->Get_Pos(), { 0, 0, -1 }, 4, 1, 0);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 30, 2, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->Get_Component<CTransform>()->Get_Pos(), { -1, 1, 0 }, 30, 2, 1);

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_1")->SetTrigger(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_2")->SetTrigger(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_3")->SetTrigger(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->SetTrigger(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_4")->Set_Loop();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->SetTrigger(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CFloatingCube_5")->Set_Loop();
}

void TestSceneHW::DirectionSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_1")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_1")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 0.f, 30.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_2")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_2")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 0.f, 30.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_3")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_3")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 0.f, 30.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_4")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_4")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 0.f, 30.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_5")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_5")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 0.f, 30.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_6")->Set_Info(Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_6")->Get_Component<CTransform>()->Get_Pos(), { 0, 1, 0 }, 0.f, 30.f);


}

void TestSceneHW::SlotSet()
{
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_1")->Set_Info(m_pPlayer, 1, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_2")->Set_Info(m_pPlayer, 1, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_3")->Set_Info(m_pPlayer, 2, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_4")->Set_Info(m_pPlayer, 2, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Set_Info(m_pPlayer, 1, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Set_Info(m_pPlayer, 1, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Set_Info(m_pPlayer, 2, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_4")->Set_Info(m_pPlayer, 2, 2);
}

