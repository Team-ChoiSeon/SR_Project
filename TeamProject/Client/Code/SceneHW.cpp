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
#include "CPickSwitch.h"
#include "CSlotCube.h"
#include "CSlotSensor.h"


SceneHW::SceneHW(LPDIRECT3DDEVICE9 pGraphicDev) 
	: CScene(pGraphicDev)
{
}

SceneHW::~SceneHW()
{
}

HRESULT SceneHW::Ready_Scene()
{
	Init_Layers();

	//===========================================================================================================//
	//Create Objects
	m_pPlayer = CMainPlayer::Create(m_pGraphicDev);
	m_pFFCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	m_pFloatingCube = CFloatingCube::Create(m_pGraphicDev);
	m_pFloatingCube2 = CFloatingCube::Create(m_pGraphicDev);
	CFloatingCube* pGround = CFloatingCube::Create(m_pGraphicDev);
	m_pDirectionalCube = CDirectionalCube::Create(m_pGraphicDev);
	m_pOnewayCube = CDirectionalCube::Create(m_pGraphicDev);
	m_pImpulseCube = CImpulseCube::Create(m_pGraphicDev);
	m_pPickSwitch = CPickSwitch::Create(m_pGraphicDev);
	m_pSlotCube = CSlotCube::Create(m_pGraphicDev);
	m_pSlotCube2 = CSlotCube::Create(m_pGraphicDev);
	m_pSlotCube3 = CSlotCube::Create(m_pGraphicDev);
	m_pSlotSensor = CSlotSensor::Create(m_pGraphicDev);
	m_pSlotSensor2 = CSlotSensor::Create(m_pGraphicDev);
	m_pSlotSensor3 = CSlotSensor::Create(m_pGraphicDev);
	//m_pWeightButton = CWeightButton::Create(m_pGraphicDev);
	//m_pTimerButton = CTimerButton::Create(m_pGraphicDev);



	//===========================================================================================================//


	//Input Objects into Layer
	Get_Layer(LAYER_PLAYER)->Add_GameObject(L"hwPlayer", m_pPlayer);
	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"hwffcam", m_pFFCam);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwFloatingCube", m_pFloatingCube);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwFloatingCube2", m_pFloatingCube2);
	Get_Layer(LAYER_TILE)->Add_GameObject(L"hwGround", pGround);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwDirectionalCube", m_pDirectionalCube);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwOnewayCube", m_pOnewayCube);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwImpulseCube", m_pImpulseCube);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwToggle", m_pPickSwitch);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwSlotCube", m_pSlotCube);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwSlotCube2", m_pSlotCube2);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwSlotCube3", m_pSlotCube3);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwSlotSensor", m_pSlotSensor);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwSlotSensor2", m_pSlotSensor2);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwSlotSensor3", m_pSlotSensor3);
	//Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwWeightButton", m_pWeightButton);
	//Get_Layer(LAYER_OBJECT)->Add_GameObject(L"hwTimerButton", m_pTimerButton);
	Get_Layer(LAYER_UI)->Add_GameObject(L"Crosshair", CCrosshairUIObject::Create(m_pGraphicDev));
	Get_Layer(LAYER_PLAYER)->Get_GameObject<CMainPlayer>(L"hwPlayer")->Set_Crosshair(Get_Layer(LAYER_UI)->Get_GameObject<CCrosshairUIObject>(L"Crosshair"));


	//===========================================================================================================//


	//Set Obejct Informations
	m_pPlayer->Get_Component<CTransform>()->Set_Pos({ 0.f, 30.f, 0.f });
	m_pPlayer->Get_Component<CRigidBody>()->Set_Friction(1.f);
	m_pPlayer->Get_Component<CRigidBody>()->Set_Mass(100.f);

	m_pFFCam->Set_Target(m_pPlayer);

	m_pFloatingCube->Set_Info({ -20.f, 0.f, 50.f }, { 1.f, 0.f, 0.f }, 40.f, 20.f, 1.5f);
	m_pFloatingCube2->Set_Info({ 20.f, 10.f, 30.f }, { 0.f, 1.f, 0.f }, 20.f, 10.f, 1.5f);
	pGround->Set_Info({ 0.f, -20.f, 0.f }, { 1.f, 0.f, 0.f }, 1.f, 0.f, 0.f);
	pGround->Get_Component<CTransform>()->Set_Scale({ 100.f, 1.f, 100.f });
	m_pDirectionalCube->Set_Info({ 0.f, 0.f, 30.f }, { 1.f, 0.f, 0.f }, -10.f, 10.f);
	m_pOnewayCube->Set_Info({ -10.f, 0.f, 30.f }, { 1.f, 0.f, 0.f }, 20.f);
	m_pImpulseCube->Set_Info({ 0.f, 0.f, 20.f });
	m_pPickSwitch->Get_Component<CTransform>()->Set_Pos({ 50.f, -15.f, 10.f });
	m_pSlotCube->Get_Component<CTransform>()->Set_Pos({ 0.f, -15.f, -10.f });
	//m_pSlotCube->Get_Component<CTransform>()->Set_Look({ 1.f, 0.f, 1.f });
	m_pSlotCube->Get_Component<CRigidBody>()->Set_AVelocity({ 0.f, 1.f, 0.f });
	m_pSlotCube->Set_Info(m_pPlayer, 0, 0);

	m_pSlotCube2->Get_Component<CTransform>()->Set_Pos({ -3.f, -15.f, -10.f });
	m_pSlotCube2->Set_Info(m_pPlayer, 0, 1);
	m_pSlotCube3->Get_Component<CTransform>()->Set_Pos({ -6.f, -15.f, -10.f });
	m_pSlotCube3->Set_Info(m_pPlayer, 1, 0);
	m_pSlotSensor->Get_Component<CTransform>()->Set_Pos({ -10.f, -15.f, -10.f });
	m_pSlotSensor->Set_Info(m_pPlayer, 0, 0);
	m_pSlotSensor2->Get_Component<CTransform>()->Set_Pos({ -15.f, -15.f, -10.f });
	m_pSlotSensor2->Set_Info(m_pPlayer, 0, 1);
	m_pSlotSensor3->Get_Component<CTransform>()->Set_Pos({ -20.f, -15.f, -10.f });
	m_pSlotSensor3->Set_Info(m_pPlayer, 1, 0);

	//m_pTimerButton->Set_Info({ -10.f, 30.f, 20.f }, 1.f, -1.f);
	//m_pWeightButton->Set_Info(10.f, -1.f);
	
	
	//===========================================================================================================//

	
	//Set Object Rigid
	m_pFloatingCube->Get_Component<CRigidBody>()->Set_Friction(0.f);
	m_pFloatingCube->Get_Component<CRigidBody>()->Set_Mass(10.f);
	m_pFloatingCube->Get_Component<CRigidBody>()->Set_Bounce(0.f);
	m_pFloatingCube->Get_Component<CRigidBody>()->Set_OnGround(true);
	m_pFloatingCube->Get_Component<CRigidBody>()->Set_UseGravity(false);
	m_pFloatingCube->Set_Loop();

	m_pFloatingCube2->Get_Component<CRigidBody>()->Set_Friction(0.f);
	m_pFloatingCube2->Get_Component<CRigidBody>()->Set_Mass(10.f);
	m_pFloatingCube2->Get_Component<CRigidBody>()->Set_Bounce(0.f);
	m_pFloatingCube2->Get_Component<CRigidBody>()->Set_OnGround(true);
	m_pFloatingCube2->Get_Component<CRigidBody>()->Set_UseGravity(false);
	m_pFloatingCube2->Set_Loop();

	pGround->Get_Component<CRigidBody>()->Set_Friction(0.f);
	pGround->Get_Component<CRigidBody>()->Set_Mass(1000.f);
	pGround->Get_Component<CRigidBody>()->Set_Bounce(0.1f);
	pGround->Get_Component<CRigidBody>()->Set_OnGround(true);
	pGround->Get_Component<CRigidBody>()->Set_UseGravity(false);
	pGround->Get_Component<CCollider>()->Set_ColTag(ColliderTag::GROUND);
	pGround->Get_Component<CCollider>()->Set_ColType(ColliderType::PASSIVE);


	m_pDirectionalCube->Get_Component<CRigidBody>()->Set_Friction(0.f);
	m_pDirectionalCube->Get_Component<CRigidBody>()->Set_Mass(10.f);
	m_pDirectionalCube->Get_Component<CRigidBody>()->Set_Bounce(0.1f);
	m_pDirectionalCube->Get_Component<CRigidBody>()->Set_OnGround(true);
	m_pDirectionalCube->Get_Component<CRigidBody>()->Set_UseGravity(false);

	m_pOnewayCube->Get_Component<CRigidBody>()->Set_Friction(0.f);
	m_pOnewayCube->Get_Component<CRigidBody>()->Set_Mass(10.f);
	m_pOnewayCube->Get_Component<CRigidBody>()->Set_Bounce(0.1f);
	m_pOnewayCube->Get_Component<CRigidBody>()->Set_OnGround(true);
	m_pOnewayCube->Get_Component<CRigidBody>()->Set_UseGravity(false);

	m_pImpulseCube->Get_Component<CRigidBody>()->Set_OnGround(false);
	m_pImpulseCube->Get_Component<CRigidBody>()->Set_UseGravity(true);
	m_pImpulseCube->Get_Component<CRigidBody>()->Set_Bounce(0.9f);
	m_pImpulseCube->Get_Component<CRigidBody>()->Set_Friction(0.1f);
	m_pImpulseCube->Get_Component<CRigidBody>()->Set_Mass(1.f);

	//m_pSlotCube->Get_Component<CRigidBody>()->Set_OnGround(false);
	//m_pSlotCube->Get_Component<CRigidBody>()->Set_UseGravity(true);
	//m_pSlotCube->Get_Component<CRigidBody>()->Set_Bounce(0.f);
	//m_pSlotCube->Get_Component<CRigidBody>()->Set_Friction(0.1f);
	//m_pSlotCube->Get_Component<CRigidBody>()->Set_Mass(1.f);

	//m_pSlotSensor->Get_Component<CRigidBody>()->Set_OnGround(true);
	//m_pSlotSensor->Get_Component<CRigidBody>()->Set_UseGravity(false);
	//m_pSlotSensor->Get_Component<CRigidBody>()->Set_Bounce(0.f);
	//m_pSlotSensor->Get_Component<CRigidBody>()->Set_Friction(0.1f);
	//m_pSlotSensor->Get_Component<CRigidBody>()->Set_Mass(1.f);




	//m_pTimerButton->Get_Component<CRigidBody>()->Set_Friction(0.f);
	//m_pTimerButton->Get_Component<CRigidBody>()->Set_Mass(10.f);
	//m_pTimerButton->Get_Component<CRigidBody>()->Set_Bounce(0.1f);
	//m_pTimerButton->Get_Component<CRigidBody>()->Set_OnGround(true);
	//m_pTimerButton->Get_Component<CRigidBody>()->Set_UseGravity(false);



	//===========================================================================================================//

	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"Crosshair"));
	
	CCameraMgr::Get_Instance()->Set_MainCamera(m_pFFCam);

	for (auto& pLayer : m_umLayer)
		pLayer.second->Ready_Layer();


	CSoundMgr::Get_Instance()->Load_Sound("Zelda", "../Bin/Resource/Sound/Zelda.wav");
	CSoundMgr::Get_Instance()->Play("Zelda", "BGM", true);
	//m_fTime = 100000.f;
	return S_OK;
}

SceneHW* SceneHW::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneHW* pScene = new SceneHW(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");	
		return nullptr;
	}

	return pScene;
}

int SceneHW::Update_Scene(const _float& fTimeDelta)
{

	CScene::Update_Scene(fTimeDelta);

	//===========================================================================================================//
	//Debugging Codes

	_vec3 playerpos = m_pPlayer->GetPos();
	_vec3 fcubepos = m_pFloatingCube->Get_Component<CTransform>()->Get_Pos();
	_vec3 dcubepos = m_pDirectionalCube->Get_Component<CTransform>()->Get_Pos();
	_vec3 pswitchpos = m_pPickSwitch->Get_Component<CTransform>()->Get_Pos();
	_vec3 sensorpos = m_pSlotSensor->Get_Component<CTransform>()->Get_Pos();
	_vec3 scubepos = m_pSlotCube->Get_Component<CTransform>()->Get_Pos();
	_bool trigger1 = m_pSlotSensor->Get_SensorState();
	_bool trigger2 = m_pSlotSensor2->Get_SensorState();
	_bool trigger3 = m_pSlotSensor3->Get_SensorState();

	if (trigger1)
		OutputDebugStringW(L"slot1 : True \n");
	else
		OutputDebugStringW(L"slot1 : False\n");
	if (trigger2)
		OutputDebugStringW(L"slot2 : True \n");
	else
		OutputDebugStringW(L"slot2 : False\n");
	if (trigger3)
		OutputDebugStringW(L"slot3 : True \n");
	else
		OutputDebugStringW(L"slot3 : False\n");
	//wchar_t buf1[128];
	//swprintf_s(buf1, 128, L"Sensor Pos : %.3f, %.3f, %.3f\n", sensorpos.x, sensorpos.y, sensorpos.z);
	//OutputDebugStringW(buf1);

	//wchar_t buf2[128];
	//swprintf_s(buf2, 128, L"SCube Pos : %.3f, %.3f, %.3f\n", scubepos.x, scubepos.y, scubepos.z);
	//OutputDebugStringW(buf2);

	//wchar_t buf3[128];
	//swprintf_s(buf3, 128, L"Player Pos : %.3f, %.3f, %.3f\n", playerpos.x, playerpos.y, playerpos.z);
	//OutputDebugStringW(buf3);

	//wchar_t bposbuf[128];
	//swprintf_s(bposbuf, 128, L"TButton Pos : %.3f, %.3f, %.3f\n", pswitchpos.x, pswitchpos.y, pswitchpos.z);
	//OutputDebugStringW(bposbuf);


	//===========================================================================================================//

	return 0;
}

void SceneHW::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

void SceneHW::Free()
{
	CScene::Free();
}

