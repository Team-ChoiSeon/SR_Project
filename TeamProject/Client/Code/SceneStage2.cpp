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
#include "CZoneSensor.h"

#include "SceneStage3.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"
#include "CSkyBox.h"
#include "CScenePanel.h"

SceneStage2::SceneStage2(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{
}

SceneStage2::~SceneStage2()
{
}

HRESULT SceneStage2::Ready_Scene()
{

	for (int i = 0; i < 15; i++)
	{
		m_vbPanels.push_back(false);
	}


	CSoundMgr::Get_Instance()->Load_Sound("BGM1", "../Bin/Resource/Sound/background.wav");

	CSoundMgr::Get_Instance()->Set_Volume("BGM1", 0.5f);
	CSoundMgr::Get_Instance()->Play("BGM1", "BGM", true);
	
	Init_Layers();
	for (auto& tile : Get_Layer(LAYER_TILE)->Get_ObjVec()) {
		tile.pObj->Get_Component<CRigidBody>()->Set_UseGravity(false);
	}

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

	pCam->Add_Component<CSkyBox>(ID_DYNAMIC, m_pGraphicDev);
	pCam->Get_Component<CSkyBox>()->Set_Texture(L"Sky_Test2.dds");
	pCam->Get_Component<CTransform>()->Set_Scale({ 500,500,500 });

	CScenePanel* uiPanel = CScenePanel::Create(m_pGraphicDev);
	uiPanel->Set_ObjectInfo(false);
	Get_Layer(LAYER_UI)->Add_GameObject(L"uiPanel", uiPanel);

	FloatingSet();
	DirectionSet();
	SlotSet();
	StairSet();

	CSoundMgr::Get_Instance()->Mute_Group("SFX", true);
	m_bSfxMuted = true;
	m_fMuteTimer = 3.f;
	
	return S_OK;
}

_int SceneStage2::Update_Scene(const _float& fTimeDelta)
{
	if (m_bSfxMuted) {
		m_fMuteTimer -= fTimeDelta;
		if (m_fMuteTimer <= 0.0f) {
			CSoundMgr::Get_Instance()->Mute_Group("SFX", false);
			m_bSfxMuted = false;
		}
	}

	if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CSceneGate>(L"CSceneGate_1")->Get_InGate()) {
		CScene* pScene = SceneStage3::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
		CSoundMgr::Get_Instance()->Stop_Group("BGM");
		CSoundMgr::Get_Instance()->Stop_Group("SFX");
		CSoundMgr::Get_Instance()->Stop_Group("ENV");
		CCollisionMgr::Get_Instance()->Clear();
		CRenderMgr::Get_Instance()->Clear();
	}
	else {

		Set_Triggers();

		CScene::Update_Scene(fTimeDelta);
	}

	//for (int i = 1; i < 6; i++) {
	//	wstring name = L"CQuestCube_" + to_wstring(i);
	//	CSlotCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(name);
	//	m_bClear = cube->Get_Slotted();
	//}
	//
	//if (m_bClear) {
	//	CTestTile* tile = Get_Layer(LAYER_TILE)->Get_GameObject<CTestTile>(L"CTestTile_4");
	//	_vec3 pos = tile->Get_Component<CTransform>()->Get_Pos();
	//	tile->Get_Component<CTransform>()->Set_PosY(pos.y+fTimeDelta);
	//}
	return 0;
}

void SceneStage2::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void SceneStage2::FloatingSet()
{
	CFloatingCube* pMoveCube1_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube1_1");
	pMoveCube1_1->Get_Component<CRigidBody>()->Set_UseGravity(false);
	pMoveCube1_1->Get_Component<CRigidBody>()->Set_OnGround(true);
	pMoveCube1_1->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	pMoveCube1_1->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	pMoveCube1_1->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);
	pMoveCube1_1->Set_Info({ 1.f, 0.f, 0.f }, 39.f, 15.f, 0.5f);
	pMoveCube1_1->Set_Loop();
	pMoveCube1_1->SetTrigger(true);

	CFloatingCube* pMainQuestMove_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMainQuestMove_1");
	CFloatingCube* pMainQuestMove_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMainQuestMove_2");

	pMainQuestMove_1->Set_Info(pMainQuestMove_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 10.f, 2.f, 0.5f);
	pMainQuestMove_2->Set_Info(pMainQuestMove_2->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 3.f, 2.f, 0.5f);

	for (int i = 1; i < 5; i++)
	{
		wstring name = L"CStageDoorMove_" + to_wstring(i);
		CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(name);
		cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 10.f, 2.f, 0.5f);
	}
	for (int i = 1; i < 5; i++)
	{
		wstring name = L"CDoorMove3_" + to_wstring(i);
		CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(name);
		cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 15.f, 2.f, 0.5f);
	}

	CFloatingCube* CMoveCube3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube3_1");
	CMoveCube3_1->Set_Info(CMoveCube3_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 3.f, 2.f, 0.5f);

	CFloatingCube* CMoveCube3_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube3_2");
	CMoveCube3_2->Set_Info(CMoveCube3_2->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 10.f, 2.f, 0.5f);

	CFloatingCube* CDoorMove4_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove4_1");
	CDoorMove4_1->Set_Info(CDoorMove4_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 15.f, 2.f, 0.5f);

}

void SceneStage2::DirectionSet()
{
	for (int i = 0; i < 9; i++) {
		wstring name = L"CDcube1_" + to_wstring(i);
		CDirectionalCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(name);
		cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
		cube->Get_Component<CRigidBody>()->Set_OnGround(true);
		cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
		cube->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
		cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);

		if (i == 0)
			cube->Set_Info({ 0.f, 1.f, 0.f }, 0.f, 15.f);
		else if (i > 4)
			cube->Set_Info({ 0.f, 0.f, -1.f }, 0.f, 5.f);
		else
			cube->Set_Info({ 0.f, 0.f, 1.f }, 0.f, 5.f);
	}

	for (int i = 1; i < 4; i++) {
		wstring name = L"CDcube3_" + to_wstring(i);
		CDirectionalCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(name);

		cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 0.f, 4.3f);
	}

	CDirectionalCube* CDirectionalCube_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_1");

	CDirectionalCube_1->Set_Info(CDirectionalCube_1->Get_Component<CTransform>()->Get_Pos(), { 0.f, 1.f, 0.f }, 0.f, 4.f);
	
}

void SceneStage2::SlotSet()
{
	auto pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"MainPlayer");

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
		cube->Set_Info( i, i);
	}

	CSlotSensor* CSlotSensor3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor3_1");
	CSlotSensor3_1->Set_Info(pPlayer, 0, 0);
	CSlotSensor3_1->Set_PlayerPick(false);

	CSlotCube_Auto* SlotCube_Auto3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube_Auto>(L"CSlotCube_Auto3_1");
	SlotCube_Auto3_1->Set_Info(0, 0);

	CSlotSensor* CSlotSensor3_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor3_2");
	CSlotSensor3_2->Set_Info(pPlayer, 1, 1);
	CSlotSensor3_2->Set_PlayerPick(false);

	CSlotCube_Auto* SlotCube_Auto3_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube_Auto>(L"CSlotCube_Auto3_2");
	SlotCube_Auto3_2->Set_Info(1, 1);


	CSlotSensor* CSlotSensor4_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor4_1");
	CSlotSensor4_1->Set_Info(pPlayer, 2, 2);
	CSlotSensor4_1->Set_PlayerPick(false);

	CSlotCube_Auto* CSlotCube_Auto4_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube_Auto>(L"CSlotCube_Auto4_1");
	CSlotCube_Auto4_1->Set_Info(2, 2);

	CSlotSensor* CSlotSensor4_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor4_2");
	CSlotSensor4_2->Set_Info(pPlayer, 8, 8);

	CSlotCube* CSlotCube4_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube4_1");
	CSlotCube4_1->Set_Info(pPlayer, 8, 8);
}

void SceneStage2::StairSet()
{
	CStairBlock* pStaircube1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CStairBlock>(L"CStair2_1");
	pStaircube1->Set_Distance(-30.f);
	CStairBlock* pStaircube2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CStairBlock>(L"CStair2_2");
	pStaircube2->Set_Distance(18.f);
}

void SceneStage2::Set_Triggers()
{
	_int iMainQuest = 0;
	for (int i = 1; i < 6; i++) {
		wstring name = L"CSlotQuest_" + to_wstring(i);
		CSlotSensor* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(name);
		if (cube->Get_SensorState())
		{
			if (i < 5)
			{
				wstring DoorName = L"CStageDoorMove_" + to_wstring(i);
				CFloatingCube* DoorCube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(DoorName);
				DoorCube->PlayElevatorSound();
				DoorCube->SetTrigger(true);
			}

			++iMainQuest;
		}
	}

	////Panel
	//if (m_bSceneFirst)
	//{
	//	auto uiPanel = Get_Layer(LAYER_UI)->Get_GameObject<CScenePanel>(L"uiPanel");
	//	uiPanel->Set_StageHint(L"문을 끌어당ru 올려보세요");
	//	m_bSceneFirst = false;
	//}

	CFloatingCube* pMainQuestMove_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMainQuestMove_1");
	CFloatingCube* pMainQuestMove_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMainQuestMove_2");
	CSlotSensor* CSlotQuest_5 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotQuest_5");
	pMainQuestMove_1->PlayElevatorSound();
	pMainQuestMove_2->PlayElevatorSound();

	if (iMainQuest == 5) {
		pMainQuestMove_1->SetTrigger(true);
		pMainQuestMove_2->SetTrigger(true);
	}
	else {
		pMainQuestMove_1->SetTrigger(false);
		pMainQuestMove_2->SetTrigger(false);
	}
	///

	_bool bPickSwitch3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch_3_1")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_1")->SetTrigger(bPickSwitch3_1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_1")->PlayElevatorSound();

	CSlotSensor* CSlotSensor3_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor3_1");
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_2")->SetTrigger(CSlotSensor3_1->Get_SensorState());
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube3_1")->SetTrigger(CSlotSensor3_1->Get_SensorState());
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_2")->PlayElevatorSound();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube3_1")->PlayElevatorSound();

	for (int i = 1; i < 5; ++i)
	{
		wstring name = L"CPickSwitch3_2_" + to_wstring(i);
		auto* pSwitch = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(name);

		if (pSwitch->Get_SwitchState())
		{
			if (find(InputSwitch3_1.begin(), InputSwitch3_1.end(), i) == InputSwitch3_1.end())
			{
				InputSwitch3_1.push_back(i);
			}
		}
	}
	if (InputSwitch3_1.size() == 4)
	{
		if (InputSwitch3_1 == AnswerSwitch3_1)
		{
			auto* pSwitch = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch3_2_4");
			Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_3")->SetTrigger(true);
			Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_3")->PlayElevatorSound();
		}
		else
		{
			for (int i = 1; i < 5; ++i) {
				wstring name = L"CPickSwitch3_2_" + to_wstring(i);
				auto* pSwitch = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(name);
				pSwitch->Set_SwitchState(false);
			}
		}
		InputSwitch3_1.clear();
	}

	// 
	_bool bPickSwitch3_3 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CPickSwitch>(L"CPickSwitch3_3")->Get_SwitchState();
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube3_2")->SetTrigger(bPickSwitch3_3);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CMoveCube3_2")->PlayElevatorSound();

	CSlotSensor* CSlotSensor3_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor3_2");
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_4")->SetTrigger(CSlotSensor3_2->Get_SensorState());
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove3_4")->PlayElevatorSound();
	// 

	_bool bSlotSensor4_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor4_1")->Get_SensorState();
	_bool bSlotSensor4_2 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor4_2")->Get_SensorState();

	if (bSlotSensor4_1 && bSlotSensor4_2) {
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove4_1")->SetTrigger(true);
		Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"CDoorMove4_1")->PlayElevatorSound();
	}

	/// 

	auto puiPanel = Get_Layer(LAYER_UI)->Get_GameObject<CScenePanel>(L"uiPanel");
	if (!m_vbPanels[0]&& Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_1")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"문을 끌어당겨 올려보세요");
		m_vbPanels[0] = true;
	}

	if (!m_vbPanels[1] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_2")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"큐브를 들어 올바른 색의 위치에 배치해보세요");
		m_vbPanels[1] = true;
	}

	CSlotSensor* CSlotQuest_1 = Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotQuest_1");
	if (!m_vbPanels[2] && CSlotQuest_1->Get_SensorState()) {
		puiPanel->Set_StageHint(L"이제 남은 색깔 큐브를 찾아오세요");
		m_vbPanels[2] = true;
	}

	if (!m_vbPanels[3] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_4")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"녹슨 발판을 밟아 노란색 색깔 큐브를 찾으세요");
		m_vbPanels[3] = true;
	}

	if (!m_vbPanels[4] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_5")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"검은 발판을 끌어서 길을 만들어 초록색 색깔 큐브를 찾으세요");
		m_vbPanels[4] = true;
	}

	if (!m_vbPanels[5] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_6")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"미로에서 큐브 방의 문을 열수 있는 장치를 찾으세요");
		m_vbPanels[5] = true;
	}

	if (!m_vbPanels[6] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_7")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"큐브를 밀어 올바른 위치에 배치하세요");
		m_vbPanels[6] = true;
	}

	if (!m_vbPanels[7] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_8")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"큐브를 잡아 올바른 위치에 배치하세요");
		m_vbPanels[7] = true;
	}

	if (!m_vbPanels[8] && bSlotSensor4_1 && bSlotSensor4_2) {
		puiPanel->Set_StageHint(L"이제 하늘색 색깔 큐브를 찾으세요");
		m_vbPanels[8] = true;
	}

	if (!m_vbPanels[9] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_10")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"퍼즐을 풀어 핑크색 색깔 큐브를 찾으세요 \n문을 열수 있는 버튼 누르기");
		m_vbPanels[9] = true;
	}

	if (!m_vbPanels[10] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_11")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"눈앞의 거대한 녹슨 큐브를 밀어보세요");
		m_vbPanels[10] = true;
	}
	if (!m_vbPanels[11] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_12")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"힌트를 찾아 버튼을 순서에 맞게 누르세요");
		m_vbPanels[11] = true;
	}
	if (!m_vbPanels[12] && Get_Layer(LAYER_OBJECT)->Get_GameObject<CZoneSensor>(L"CZoneP_13")->Get_SensorState()) {
		puiPanel->Set_StageHint(L"검은 큐브를 막고있는 장치를 풀 버튼을 찾아 누르세요");
		m_vbPanels[12] = true;
	}
	if (!m_vbPanels[13] && bPickSwitch3_3) {
		puiPanel->Set_StageHint(L"검은 큐브를 밀고 바닥을 끌어 올려 길을 만들어 검은 큐브를 가장위에 배치하세요");
		m_vbPanels[13] = true;
	}
	if (!m_vbPanels[14] && iMainQuest == 5) {
		puiPanel->Set_StageHint(L"다음 장소로 갈 수 있는 문을 통해 이동하세요.");
		m_vbPanels[14] = true;
	}
	
}

SceneStage2* SceneStage2::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneStage2* pScene = new SceneStage2(pGraphicDev);

	return pScene;
}

void SceneStage2::Free()
{
	CSoundMgr::Get_Instance()->Stop("BGM1");
	CScene::Free();
}