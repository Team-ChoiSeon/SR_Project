#pragma once
#include "pch.h"
#include "SceneHS.h"

#include "CTimeMgr.h"
#include "CInputMgr.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CUiMgr.h"
#include "CResourceMgr.h"
#include "CPickingMgr.h"
#include "CCollisionMgr.h"

#include "CPlayer.h"
#include "CMainPlayer.h"
#include "CLightObject.h"
#include "CTestLightMeshObject.h"
#include "CCrosshairUIObject.h"
#include "DummyCube.h"
#include "CDirectionalCube.h"
#include "CTestTile.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"


SceneHS::SceneHS(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pGraphicDev(pGraphicDev)
{

}

SceneHS::~SceneHS()
{
}

HRESULT SceneHS::Ready_Scene()
{
	CUiMgr::Get_Instance()->Ready_UiMgr();
	Init_Layers();

	CTestTile* pTile = CTestTile::Create(m_pGraphicDev);
	pTile->Get_Component<CTransform>()->Set_Scale({ 50.f, 10.f, 50.f });
	pTile->Get_Component<CTransform>()->Set_PosY(-20.f);
	pTile->Get_Component<CRigidBody>()->Set_OnGround(true);
	pTile->Get_Component<CRigidBody>()->Set_UseGravity(false);


	CDirectionalCube* pOnewayCube = CDirectionalCube::Create(m_pGraphicDev);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwOnewayCube", pOnewayCube);
	pOnewayCube->Set_Info({ -10.f, 0.f, 30.f }, { 1.f, 0.f, 0.f }, 20.f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_Friction(0.f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_Mass(10.f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_Bounce(0.1f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_OnGround(true);
	pOnewayCube->Get_Component<CRigidBody>()->Set_UseGravity(false);

	//////////////////
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", CMainPlayer::Create(m_pGraphicDev));

	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"Dummy", DummyCube::Create(m_pGraphicDev));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwDirectionalCube", CDirectionalCube::Create(m_pGraphicDev));
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwDirectionalCube"))->Set_Info({ 5.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, -10.f, 10.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"hwDirectionalCube"))->Set_Grab(true);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"GroundDummy", (pTile));

	Get_Layer(LAYER_LIGHT)->Add_GameObject(L"TestLightMesh", CTestLightMeshObject::Create(m_pGraphicDev));
	Get_Layer(LAYER_LIGHT)->Add_GameObject(L"LightObject", CLightObject::Create(m_pGraphicDev));
	
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));

	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"ffcam", CFirstviewFollowingCamera::Create(m_pGraphicDev));
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"dummycam", CFirstviewFollowingCamera::Create(m_pGraphicDev));

	/////////////////////
	Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"dummycam")->Set_Target(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Dummy"));
	Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam")->Set_Target(Get_Layer(LAYER_PLAYER)->Get_GameObject(L"Player"));

	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	CPickingMgr::Get_Instance()->Ready_Picking(m_pGraphicDev, g_hWnd);
	CCameraMgr::Get_Instance()->Set_MainCamera(Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam"));

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int SceneHS::Update_Scene(const _float& fTimeDelta)
{
	CPickingMgr::Get_Instance()->Update_Picking(fTimeDelta);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	CGameObject* PickObj = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"Player")->Get_PickObj();
	auto* pPickCubeObj = dynamic_cast<CDirectionalCube*>(PickObj);
	
	if (pPickCubeObj) {
		pPickCubeObj->Set_Grab(false);
	}

	if (PickObj)
	{
		if (Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"Player")->Get_Hold()) {
			Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair")->Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOLD);



			if (pPickCubeObj) {
				pPickCubeObj->Set_Grab(true);
				pPickCubeObj->Set_CursorVec(Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"Player")->Get_DragDistance());
			}
		}
		else {
			Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair")->
				Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_HOVER);
		}
	}
	else {
		Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair")->
			Set_State(CCrosshairUIObject::CROSSHAIR_STATE::CROSS_DEFAULT);
	}

	CCollisionMgr::Get_Instance()->Update_Collision();
	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);

	// m_pLightObject->Update_GameObject(fTimeDelta);
	// m_pTestLightMesh->Update_GameObject(fTimeDelta);

	return 0;
}

void SceneHS::LateUpdate_Scene(const _float& fTimeDelta)
{
	CPickingMgr::Get_Instance()->LateUpdate_Picking(fTimeDelta);

	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);

	CLightMgr::Get_Instance()->UpdateLights(CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>()->Get_Pos());
	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);

	// m_pLightObject->LateUpdate_GameObject(fTimeDelta);
	// m_pTestLightMesh->LateUpdate_GameObject(fTimeDelta);
	
	//Engine::CLightMgr::Get_Instance()->UpdateLights({ 0.f, 0.f, -10.f });
}

SceneHS* SceneHS::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneHS* pScene = new SceneHS(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneHS::Free()
{	
	Clear_Layers();

	CPickingMgr::Destroy_Instance();
	CUiMgr::Destroy_Instance();
	CResourceMgr::Destroy_Instance();

	CScene::Free();
}