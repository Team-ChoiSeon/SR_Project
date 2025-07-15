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
#include "CSceneMgr.h"
#include "CRenderMgr.h"

#include "CPlayer.h"
#include "CMainPlayer.h"
#include "CLightObject.h"
#include "CTestLightMeshObject.h"
#include "CCrosshairUIObject.h"
#include "DummyCube.h"
#include "CDirectionalCube.h"
#include "CTestTile.h"
#include "CSceneGate.h"
#include "CCinematicCamera.h"
#include "CDollyCamera.h"

#include "SceneHW.h"
#include "SceneBG.h"

#include "CCamera.h"
#include "CFirstviewFollowingCamera.h"


SceneHS::SceneHS(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

SceneHS::~SceneHS()
{
}

HRESULT SceneHS::Ready_Scene()
{
	Init_Layers();

	CMainPlayer* pPlayer = CMainPlayer::Create(m_pGraphicDev);
	pPlayer->Get_Component<CTransform>()->Set_Pos({ -20.f, 20.f, -20.f });

	CTestTile* pTile = CTestTile::Create(m_pGraphicDev);
	pTile->Get_Component<CTransform>()->Set_Scale({ 50.f, 10.f, 50.f });
	pTile->Get_Component<CTransform>()->Set_PosY(-20.f);
	pTile->Get_Component<CRigidBody>()->Set_OnGround(true);
	pTile->Get_Component<CRigidBody>()->Set_UseGravity(false);

	CTestTile* pWall = CTestTile::Create(m_pGraphicDev);
	pWall->Get_Component<CTransform>()->Set_Scale({ 50.f, 10.f, 10.f });
	pWall->Get_Component<CTransform>()->Set_Pos({ -10.f, 0.f, 100.f });
	pWall->Get_Component<CRigidBody>()->Set_OnGround(true);
	pWall->Get_Component<CRigidBody>()->Set_UseGravity(false);

	CDirectionalCube* pOnewayCube = CDirectionalCube::Create(m_pGraphicDev);
	pOnewayCube->Set_Info({ -10.f, 0.f, 30.f }, { 1.f, 0.f, 0.f }, 20.f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_Friction(0.f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_Mass(10.f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_Bounce(0.1f);
	pOnewayCube->Get_Component<CRigidBody>()->Set_OnGround(true);
	pOnewayCube->Get_Component<CRigidBody>()->Set_UseGravity(false);
	
	CDirectionalCube* pDirectionalCube = CDirectionalCube::Create(m_pGraphicDev);
	pDirectionalCube->Set_Info({ 5.f, 0.f, 0.f }, { 1.f, 0.f, 0.f }, -10.f, 10.f);
	pDirectionalCube->Set_Grab(true);

	CCrosshairUIObject* pCrosshair = CCrosshairUIObject::Create(m_pGraphicDev);

	CFirstviewFollowingCamera* ffcam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	CFirstviewFollowingCamera* dummycam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	CCinematicCamera* pCineCam = CCinematicCamera::Create(m_pGraphicDev);
	CDollyCamera* pDollyCam = CDollyCamera::Create(m_pGraphicDev);
	pDollyCam->Get_Component<CTransform>()->Set_Pos(pOnewayCube->Get_Component<CTransform>()->Get_Pos());
	pDollyCam->Start_Dolly(pOnewayCube, _vec3(0.f, 0.f, -20.f), D3DX_PI * 0.15f, 10.f);

	//////////////////
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", pPlayer);
	CSceneMgr::Get_Instance()->Set_Player(pPlayer);

	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"Dummy", DummyCube::Create(m_pGraphicDev));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwDirectionalCube", pDirectionalCube);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"GroundDummy", (pTile));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"SceneGate", CSceneGate::Create(m_pGraphicDev));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"Wall", (pWall));
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwOnewayCube", pOnewayCube);

	Get_Layer(LAYER_LIGHT)->Add_GameObject(L"TestLightMesh", CTestLightMeshObject::Create(m_pGraphicDev));
	Get_Layer(LAYER_LIGHT)->Add_GameObject(L"LightObject", CLightObject::Create(m_pGraphicDev));
	
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", pCrosshair);

	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"ffcam", ffcam);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"dummycam", dummycam);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"DollyCam", pDollyCam);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"CineCam", pCineCam);

	/////////////////////
	pCineCam->Set_Target(pPlayer);
	Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"dummycam")->Set_Target(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Dummy"));
	Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam")->Set_Target(Get_Layer(LAYER_PLAYER)->Get_GameObject(L"Player"));


	pPlayer->Set_Crosshair(pCrosshair);
	
	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	
	CCameraMgr::Get_Instance()->Set_MainCamera(Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam"));
	//CCameraMgr::Get_Instance()->Set_MainCamera(pDollyCam);
	//CCameraMgr::Get_Instance()->Set_MainCamera(pCineCam);

	return S_OK;
}

_int SceneHS::Update_Scene(const _float& fTimeDelta)
{
	if (CCameraMgr::Get_Instance()->Get_MainCamera() == Get_Layer(LAYER_CAMERA)->Get_GameObject<CDollyCamera>(L"DollyCam"))
	{
		if (!Get_Layer(LAYER_CAMERA)->Get_GameObject<CDollyCamera>(L"DollyCam")->Get_DollyEnd())
		{
			End_Cinematic();
		}
	}

	if (Get_Layer(LAYER_OBJECT)->Get_GameObject<CSceneGate>(L"SceneGate")->Get_InGate()) {
		CScene* pScene = SceneBG::Create(m_pGraphicDev);
		CSceneMgr::Get_Instance()->Set_Scene(pScene);
		CCollisionMgr::Get_Instance()->Clear();
		CRenderMgr::Get_Instance()->Clear();
	}
	else {
		CScene::Update_Scene(fTimeDelta);
	}

	// m_pLightObject->Update_GameObject(fTimeDelta);
	// m_pTestLightMesh->Update_GameObject(fTimeDelta);
	// _vec3 playerpos = Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam")->Get_Component<CTransform>()->Get_Pos();
	// wchar_t buf1[128];
	// swprintf_s(buf1, 128, L"Player Pos : %.3f, %.3f, %.3f\n", playerpos.x, playerpos.y, playerpos.z);
	// OutputDebugStringW(buf1);

	return 0;
}

void SceneHS::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
	//CLightMgr::Get_Instance()->UpdateLights(CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>()->Get_Pos());

	// m_pLightObject->LateUpdate_GameObject(fTimeDelta);
	// m_pTestLightMesh->LateUpdate_GameObject(fTimeDelta);
	
	//Engine::CLightMgr::Get_Instance()->UpdateLights({ 0.f, 0.f, -10.f });	if (m_bInGate) {
}

void SceneHS::End_Cinematic()
{
	Get_Layer(LAYER_CAMERA)->Get_GameObject<CDollyCamera>(L"DollyCam")->End_Dolly();
	CCameraMgr::Get_Instance()->Set_MainCamera(Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam"));
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
	CScene::Free();
}