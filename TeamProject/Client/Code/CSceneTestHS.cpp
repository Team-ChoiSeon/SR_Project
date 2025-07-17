#pragma once
#include "pch.h"
#include "CSceneTestHS.h"
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

CSceneTestHS::CSceneTestHS(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

CSceneTestHS::~CSceneTestHS()
{
}

HRESULT CSceneTestHS::Ready_Scene()
{
	Init_Layers();
	//CFactory::DeSerializeScene(L"../../Scene/SampleScene1.json", this);
	//CTestTile_1
	
	Get_Layer(LAYER_OBJECT)->Get_GameObject(L"CTestTile_1")->Get_Component<CRigidBody>()->Set_OnGround(true);
	//Get_Layer(LAYER_OBJECT)->Get_GameObject(L"CTestTile_1")->Get_Component<CRigidBody>()->Set_UseGravity(false);

	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"Player", CMainPlayer::Create(m_pGraphicDev));
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"ffcam", CFirstviewFollowingCamera::Create(m_pGraphicDev));
	Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam")->Set_Target(Get_Layer(LAYER_PLAYER)->Get_GameObject(L"Player"));

	Get_Layer(LAYER_OBJECT)->Get_GameObject<CDirectionalCube>(L"CDirectionalCube_1")->Set_Info({ 10, 10, 10 }, { 1, 0, 0 }, -5.f, 5.f);

	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	CPickingMgr::Get_Instance()->Ready_Picking(m_pGraphicDev, g_hWnd);
	CCameraMgr::Get_Instance()->Set_MainCamera(Get_Layer(LAYER_CAMERA)->Get_GameObject<CFirstviewFollowingCamera>(L"ffcam"));

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();

	return S_OK;
}

_int CSceneTestHS::Update_Scene(const _float& fTimeDelta)
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

	//Debugging Codes
	_vec3 dcubepos = Get_Layer(LAYER_OBJECT)->Get_GameObject(L"CDirectionalCube_1")->Get_Component<CTransform>()->Get_Pos();

	wchar_t dcposbuf[128];
	swprintf_s(dcposbuf, 128, L"DCube Pos : %.3f, %.3f, %.3f\n", dcubepos.x, dcubepos.y, dcubepos.z);
	OutputDebugStringW(dcposbuf);


	return 0;
}

void CSceneTestHS::LateUpdate_Scene(const _float& fTimeDelta)
{
	CPickingMgr::Get_Instance()->LateUpdate_Picking(fTimeDelta);

	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);

	CCameraMgr::Get_Instance()->LateUpdate_Camera(fTimeDelta);
}

CSceneTestHS* CSceneTestHS::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CSceneTestHS* pScene = new CSceneTestHS(pGraphicDev);

	return pScene;
}

void CSceneTestHS::Free()
{
	CScene::Free();
}