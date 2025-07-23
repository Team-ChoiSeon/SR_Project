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
#include "CSlotSensor.h"
#include "CSlotCube.h"
#include "CSceneMgr.h"
#include "CParticle.h"
#include "CParticle.h"
#include "CScenePanel.h"
#include "CSkyBox.h"

SceneBG::SceneBG(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pPlayer(nullptr)
{

}

SceneBG::~SceneBG()
{
}

HRESULT SceneBG::Ready_Scene()
{
	for (auto& tile : Get_Layer(LAYER_TILE)->Get_ObjVec()) {
		tile.pObj->Get_Component<CRigidBody>()->Set_UseGravity(false);
	}
	CMainPlayer* pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"MainPlayer");
	pPlayer->Get_Component<CRigidBody>()->Set_UseGravity(true);
	pPlayer->Get_Component<CRigidBody>()->Set_OnGround(true);
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);
	
	CScenePanel* uiPanel = CScenePanel::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"uiPanel", uiPanel);

	CCrosshairUIObject* cross = CCrosshairUIObject::Create(m_pGraphicDev);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", cross);
	pPlayer->Set_Crosshair(cross);


	FFCam* pCam = FFCam::Create(m_pGraphicDev);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"MyCamera", pCam);
	pCam->Set_Target(pPlayer);
	CCameraMgr::Get_Instance()->Set_MainCamera(pCam);

	pCam->Add_Component<CSkyBox>(ID_DYNAMIC, m_pGraphicDev);
	pCam->Get_Component<CSkyBox>()->Set_Texture(L"Sky_Test2.dds");
	pCam->Get_Component<CTransform>()->Set_Scale({500,500,500 });
	SlotSet();
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

}

void SceneBG::SlotSet()
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

		cube->Set_Info(pPlayer, i, i);
	}
}

SceneBG* SceneBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneBG* pScene = new SceneBG(pGraphicDev);

	//if (FAILED(pScene->Ready_Scene()))
	//{
	//	Safe_Release(pScene);
	//	MSG_BOX("SceneBG Create Failed");
	//	return nullptr;
	//}

	return pScene;
}


void SceneBG::Free()
{
	CScene::Free();
}