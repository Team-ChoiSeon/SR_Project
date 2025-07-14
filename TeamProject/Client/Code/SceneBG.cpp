#pragma once
#include "pch.h"
#include "SceneBG.h"
#include "Engine_GUI.h"
#include "CGuiSystem.h"
#include "CFactory.h"
#include "CFirstviewFollowingCamera.h"
#include "CMainPlayer.h"
#include "CCameraMgr.h"
#include "CCollisionMgr.h"
#include "CDirectionalCube.h"
#include "CFloatingCube.h"
#include "CPickingMgr.h"
#include "CUiMgr.h"
#include "CStairBlock.h"
#include "CSceneMgr.h"

SceneBG::SceneBG(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pPlayer(nullptr)
{

}

SceneBG::~SceneBG()
{
}

HRESULT SceneBG::Ready_Scene()
{
	Init_Layers();
	CFactory::DeSerializeScene(L"../../Scene/Merge_Scene.json", this);
	//CFactory::DeSerializeScene(L"../../Scene/Jahyeon.json", this);

	//플레이어 셋
	m_pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"MainPlayer");
	m_pPlayer->Get_Component<CRigidBody>()->Set_UseGravity(true);
	m_pPlayer->Get_Component<CRigidBody>()->Set_OnGround(true);
	CSceneMgr::Get_Instance()->Set_Player(m_pPlayer);

	//크로스 헤어 셋
	CCrosshairUIObject* cross = CCrosshairUIObject::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", cross);
	CUiMgr::Get_Instance()->AddUI(cross);
	m_pPlayer->Set_Crosshair(cross);

	//카메라 셋
	FFCam* pCam = FFCam::Create(m_pGraphicDev);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	pCam->Set_Target(m_pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	TileLayer_Set();
	Step_StoneSet();
	Moving_StoneSet();
	Stair_Set();
	

	return S_OK;
}

_int SceneBG::Update_Scene(const _float& fTimeDelta)
{
	CScene::Update_Scene(fTimeDelta);
	return 0;
}

void SceneBG::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
	CGuiSystem::Get_Instance()->RegisterPanel("test", [this]() {
		Test_Panel();
	});
}

void SceneBG::TileLayer_Set()
{
	//타일 큐브 모두 중력 무효화
	for (OBJINFO obj : Get_Layer(LAYER_TILE)->Get_ObjVec()) {
		obj.pObj->Get_Component<CRigidBody>()->Set_UseGravity(false);
		obj.pObj->Get_Component<CRigidBody>()->Set_OnGround(true);
		obj.pObj->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
		obj.pObj->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	}
}

void SceneBG::Step_StoneSet()
{
	for (int i = 1; i <8; i++) {
		wstring name = L"Step_Stone" + to_wstring(i);
		CDirectionalCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(name);
		cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
		cube->Get_Component<CRigidBody>()->Set_OnGround(true);
		cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
		cube->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
		cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);
		if(i == 6)
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), {0.f, 0.f, -1.f}, 0.f, 4.f);
		else if(i==7)
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 0.f, 1.f }, 0.f, 4.f);
		else
			cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 0.f, -1.f }, 0.f, 2.f);
	}

	wstring name = L"Grabing_Stone1";
	CDirectionalCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(name);
	cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
	cube->Get_Component<CRigidBody>()->Set_OnGround(true);
	cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	cube->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	cube->Get_Component<CCollider>()->Set_BoundType(BoundingType::AABB);
	cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { -1.f, 0.f, 0.f }, 0.f, 15.f);
}

void SceneBG::Moving_StoneSet()
{
	CFloatingCube* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CFloatingCube>(L"Moving_Stone1");
	cube->Get_Component<CRigidBody>()->Set_UseGravity(false);
	cube->Get_Component<CRigidBody>()->Set_OnGround(true);
	cube->Get_Component<CCollider>()->Set_ColTag(ColliderTag::PLATFORM);
	cube->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	cube->Set_Info(cube->Get_Component<CTransform>()->Get_Pos(), { 0.f, 0.f, -1.f }, 39.f, 20.f, 0.5f);
	cube->Set_Loop();
}

void SceneBG::Stair_Set()
{
	for (int i = 1; i < 5; i++) {
		wstring name = L"Stair" + to_wstring(i);
		CStairBlock* cube = Get_Layer(LAYER_OBJECT)->Get_GameObject<CStairBlock>(name);
		cube->Set_Distance(5.f);
	}
}

void SceneBG::Test_Panel()
{
	// 간단한 GUI 창 하나 출력
	ImGui::Begin("Test Panel");
	ImGui::Text("Hello, ImGui!");
	static float f = 0.0f;
	ImGui::SliderFloat("Float Value", &f, 0.0f, 1.0f);
	ImGui::End();
}

SceneBG* SceneBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneBG* pScene = new SceneBG(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneBG Create Failed");
		return nullptr;
	}

	return pScene;
}


void SceneBG::Free()
{
	CScene::Free();
}