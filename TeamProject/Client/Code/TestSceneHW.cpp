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
#include "CWeightButton.h"
#include "CMonster.h"
#include "CTestTile.h"
#include "CCollisionMgr.h"
#include "CPlayer.h"
#include "CRigidBody.h"
#include "CTimerButton.h"
#include "CCrosshairUIObject.h"
#include "CUiMgr.h"
#include "TestSceneHW.h"
#include "CFactory.h"
#include "CMesh.h"

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
	CFactory::DeSerializeScene(L"../../Scene/TestScene_2.json", this);

	m_pFFCam = FFCam::Create(m_pGraphicDev);
	m_pPlayer = Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"CMainPlayer_1");
	//m_pPlayer = CMainPlayer::Create(m_pGraphicDev);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"FFCam", m_pFFCam);
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", m_pPlayer);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));

	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A1")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A1")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A2")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A2")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A3")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A3")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A4")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A4")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3")->Get_Component<CRigidBody>()->Set_UseGravity(false);

	Get_Layer(LAYER_TILE)->Get_GameObject(L"BaseTile")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"BaseTile")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"StartTile")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"StartTile")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"EndTile")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"EndTile")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_1")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_1")->Get_Component<CRigidBody>()->Set_UseGravity(false);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_2")->Get_Component<CRigidBody>()->Set_OnGround(true);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_2")->Get_Component<CRigidBody>()->Set_UseGravity(false);

	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A1")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A1")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A2")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A2")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A3")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A3")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A4")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A4")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"BaseTile")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"BaseTile")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"StartTile")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"StartTile")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"EndTile")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"EndTile")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_1")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_1")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_2")->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	Get_Layer(LAYER_TILE)->Get_GameObject(L"Wall_2")->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);

	const wstring basetilekey = L"Sand_008.mtl";
	const wstring floatingcubekey = L"Rock029.mtl";
	const wstring diretionalkey = L"Wood_Grain.mtl";
	//const wstring basetilekey = L"Sand_008.mtl";
	//const wstring basetilekey = L"Sand_008.mtl";


	CResourceMgr::Get_Instance()->Load_Material(basetilekey);
	CMaterial* material1 = CResourceMgr::Get_Instance()->Get_Material(basetilekey);

	CResourceMgr::Get_Instance()->Load_Material(floatingcubekey);
	CMaterial* floatingmaterial = CResourceMgr::Get_Instance()->Get_Material(floatingcubekey);
	CResourceMgr::Get_Instance()->Load_Material(diretionalkey);
	CMaterial* diretionalmtl = CResourceMgr::Get_Instance()->Get_Material(diretionalkey);
	
	Get_Layer(LAYER_TILE)->Get_GameObject(L"BaseTile")->Get_Component <CModel>()->Set_Material(material1);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A")->Get_Component <CModel>()->Set_Material(floatingmaterial);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2")->Get_Component <CModel>()->Set_Material(floatingmaterial);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3")->Get_Component <CModel>()->Set_Material(floatingmaterial);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A1")->Get_Component <CModel>()->Set_Material(diretionalmtl);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A2")->Get_Component <CModel>()->Set_Material(diretionalmtl);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A3")->Get_Component <CModel>()->Set_Material(diretionalmtl);
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A4")->Get_Component <CModel>()->Set_Material(diretionalmtl);


	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A1"))->Set_Info({ 10.f, 9.75f, 40.f }, { 1.f, 0.f, 0.f }, -10.f, 0.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A2"))->Set_Info({ -10.f, 14.f, 45.f }, { 1.f, 0.f, 0.f }, 0.f, 10.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A3"))->Set_Info({ 0.f, 8.f, 53.f }, { 0.f, 1.f, 0.f }, 0.f, 10.f);
	dynamic_cast<CDirectionalCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Directional_A4"))->Set_Info({ -20.f, 60.f, 120.f }, { 1.f, 0.5f, 0.f }, -15.f, 0.f);
	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A"))->Set_Info({ 0.f, 9.75f, 21.f }, { 0.f, 0.f, 1.f }, 20.f, 10.f, 0.f);
	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A"))->Set_Loop();
	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2"))->Set_Info({ 0.f, 22.f, 60.f }, { 3.f, 2.f, 2.f }, 40.f, 10.f, 0.f);
	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2"))->Set_Loop();
	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3"))->Set_Info({ 30.f, 40.f, 80.f }, { -6.f, 2.f, 4.f }, 70.f, 10.f, 0.f);
	dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3"))->Set_Loop();

	m_pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 30.f, 10.f });
	m_pFFCam->Set_Target(m_pPlayer);

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


	CCameraMgr::Get_Instance()->Update_Camera(m_pGraphicDev, fTimeDelta);
	CCollisionMgr::Get_Instance()->Update_Collision();


	//===========================================================================================================//
	//Debugging Codes

	//_vec3 playerpos = m_pPlayer->GetPos();
	_bool FloatingALoop = dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A"))->Get_Loop();
	_bool FloatingA2Loop = dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2"))->Get_Loop();
	_bool FloatingA3Loop = dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3"))->Get_Loop();
	_float FloatingATime = dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A"))->Get_SleepTime();
	_float FloatingA2Time = dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A2"))->Get_SleepTime();
	_float FloatingA3Time = dynamic_cast<CFloatingCube*>(Get_Layer(LAYER_OBJECT)->Get_GameObject(L"Floating_A3"))->Get_SleepTime();


	if (FloatingALoop)
		OutputDebugStringW(L"A1 Loop : True \n");
	else
		OutputDebugStringW(L"A1 Loop : False\n");
	if (FloatingA2Loop)
		OutputDebugStringW(L"A2 Loop : True \n");
	else
		OutputDebugStringW(L"A2 Loop : False\n");
	if (FloatingA3Loop)
		OutputDebugStringW(L"A3 Loop : True \n");
	else
		OutputDebugStringW(L"A3 Loop : False\n");

	wchar_t pposbuf[128];
	swprintf_s(pposbuf, 128, L"Floating A1 Time : %.3f\n", FloatingATime);
	OutputDebugStringW(pposbuf);

	wchar_t fcposbuf[128];
	swprintf_s(fcposbuf, 128, L"Floating A2 Time : %.3f\n", FloatingA2Time);
	OutputDebugStringW(fcposbuf);

	wchar_t dcposbuf[128];
	swprintf_s(dcposbuf, 128, L"Floating A3 Time : %.3f\n", FloatingA3Time);
	OutputDebugStringW(dcposbuf);

	//wchar_t bposbuf[128];
	//swprintf_s(bposbuf, 128, L"TButton Pos : %.3f, %.3f, %.3f\n", tbuttonpos.x, tbuttonpos.y, tbuttonpos.z);
	//OutputDebugStringW(bposbuf);

	//===========================================================================================================//



	return 0;
}

void TestSceneHW::LateUpdate_Scene(const _float& fTimeDelta)
{
	CPickingMgr::Get_Instance()->LateUpdate_Picking(fTimeDelta);

	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
	
	//CLightMgr::Get_Instance()->UpdateLights(CCameraMgr::Get_Instance()->Get_MainCamera()->Get_Component<CTransform>()->Get_Pos());
	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
}

void TestSceneHW::Render_Scene()
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Render_Layer();
}

void TestSceneHW::Free()
{
	Clear_Layers();
	CPickingMgr::Destroy_Instance();
	CUiMgr::Destroy_Instance();
	CResourceMgr::Destroy_Instance();

	CScene::Free();

}

