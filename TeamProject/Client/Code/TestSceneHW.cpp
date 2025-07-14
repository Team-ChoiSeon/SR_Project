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
#include "CImpulseCube.h"


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
	CFactory::DeSerializeScene(L"../../Scene/Test_6.json", this);

	m_pFFCam = FFCam::Create(m_pGraphicDev);
	m_pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1");
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"FFCam", m_pFFCam);
	//m_pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 100.f, 10.f });

	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));
	Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1")->Set_Crosshair(Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair"));


	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_1")->Set_Info(m_pPlayer, 0, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_2")->Set_Info(m_pPlayer, 0, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotSensor>(L"CSlotSensor_3")->Set_Info(m_pPlayer, 0, 3);

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Set_Info(m_pPlayer, 0, 1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Set_Info(m_pPlayer, 0, 2);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Set_Info(m_pPlayer, 0, 3);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
	//Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Get_Component<CRigidBody>()->Set_Mass(0.f);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_1")->Get_Component<CRigidBody>()->Set_OnGround(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_2")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Get_Component<CRigidBody>()->Set_UseGravity(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CSlotCube>(L"CSlotCube_3")->Get_Component<CRigidBody>()->Set_OnGround(true);
	

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_1")->Get_Component<CRigidBody>()->Set_UseGravity(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_1")->Get_Component<CRigidBody>()->Set_OnGround(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_2")->Get_Component<CRigidBody>()->Set_UseGravity(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_2")->Get_Component<CRigidBody>()->Set_OnGround(false);

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_1")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_1")->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_2")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::NONE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_2")->Get_Component<CCollider>()->Set_ColType(ColliderType::ACTIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject<CImpulseCube>(L"CImpulseCube_1")->Get_Component<CTransform>()->Set_Pos({0,10,0});
	m_pFFCam->Set_Target(m_pPlayer);
	CSceneMgr::Get_Instance()->Set_Player(m_pPlayer);

	m_pPlayer->Get_Component<CRigidBody>()->Set_OnGround(true);

	
	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	CPickingMgr::Get_Instance()->Ready_Picking(m_pGraphicDev, g_hWnd);
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);

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
	CScene::Update_Scene(fTimeDelta);

	//===========================================================================================================//
	//Debugging Codes





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
	CPickingMgr::Destroy_Instance();
	CUiMgr::Destroy_Instance();
	CResourceMgr::Destroy_Instance();

	CScene::Free();

}

