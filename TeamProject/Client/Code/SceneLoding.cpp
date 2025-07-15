#pragma once
#include "pch.h"
#include "SceneLoding.h"

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
#include "CProgressBar.h"
#include "CLodingCube.h"

#include "SceneHW.h"
#include "CScene.h"
#include "CCamera.h"
#include "CFactory.h"
#include "CFirstviewFollowingCamera.h"


SceneLoding::SceneLoding(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev), m_pGraphicDev(pGraphicDev),m_eNowStep(LOADING_STEP::READ_SCENE),layerCount(0)
{

}

SceneLoding::~SceneLoding()
{
}

HRESULT SceneLoding::Ready_Scene()
{
	Init_Layers();

	CProgressBar* pProgressBar = CProgressBar::Create(m_pGraphicDev);
	pProgressBar->Set_Progress(0.f);
	Get_Layer(LAYER_UI)->Add_GameObject(L"ProgressBar", pProgressBar);
	CUiMgr::Get_Instance()->AddUI(Get_Layer(LAYER_UI)->Get_GameObject(L"ProgressBar"));

	CLodingCube* m_pRotateCube = CLodingCube::Create(m_pGraphicDev);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"RotateCube", m_pRotateCube);

	//카메라 타겟
	CTestTile* m_pCamTargetDummy = CTestTile::Create(m_pGraphicDev);
	_vec3 vCubePos = m_pRotateCube->Get_Component<CTransform>()->Get_Pos();
	_vec3 vDummyPos = vCubePos + _vec3(-1.38f, -0.12f, -3.3f);
	//_vec3 vDummyPos = vCubePos + _vec3(-0.7f, 0.85f, -3.f);

	m_pCamTargetDummy->Get_Component<CTransform>()->Set_Pos(vDummyPos);
	m_pCamTargetDummy->Get_Component<CTransform>()->Set_ScaleY(2.f);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"CamTargetDummy", m_pCamTargetDummy);
	CFirstviewFollowingCamera* pDummyCam = CFirstviewFollowingCamera::Create(m_pGraphicDev);
	pDummyCam->Set_Target(m_pCamTargetDummy);

	Get_Layer(LAYER_CAMERA)->Add_GameObject(L"DummyCam", pDummyCam);
	Get_Layer(LAYER_OBJECT)->Add_GameObject(L"DummyTarget", m_pCamTargetDummy);
	CCameraMgr::Get_Instance()->Set_MainCamera(pDummyCam);

	return S_OK;
}

_int SceneLoding::Update_Scene(const _float& fTimeDelta)
{
	m_fProgressTimer += fTimeDelta;

	m_fProgress = min(m_fProgress, layerCount);
	float ProgressPer = static_cast<float>(m_fProgress) / layerCount;
	Get_Layer(LAYER_UI)->Get_GameObject<CProgressBar>(L"ProgressBar")->Set_Progress(ProgressPer);

	if (currentLayerIter == jLayers.end())
		m_eNowStep = LOADING_STEP::CHANGE_SCENE;

	switch (m_eNowStep)
	{
	case SceneLoding::LOADING_STEP::NONE:
		break;
	case SceneLoding::LOADING_STEP::READ_SCENE:
		break;
	case SceneLoding::LOADING_STEP::LOAD_LAYER:
		Load_Layer();
		break;
	case SceneLoding::LOADING_STEP::LOAD_OBJ:
		if(m_fProgressTimer > .1f) 
			LoadObject();
		break;
	case SceneLoding::LOADING_STEP::CHANGE_SCENE:
		Change_Scene();
		break;
	default:
		break;
	}

	CScene::Update_Scene(fTimeDelta);

	return 0;
}

void SceneLoding::LateUpdate_Scene(const _float& fTimeDelta)
{
	CScene::LateUpdate_Scene(fTimeDelta);
}

SceneLoding* SceneLoding::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneLoding* pScene = new SceneLoding(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

//우선 씬의 파일을 읽고
HRESULT SceneLoding::LoadScene(CScene* from, CScene* to)
{
	m_eNowStep = LOADING_STEP::READ_SCENE;

	wstring ScenePath = to->Get_ScenePath();

	HANDLE hFile = ::CreateFileW(
		ScenePath.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE) {
		MessageBoxW(nullptr, L" 씬 파일 열기 실패", L"Error", MB_OK);
		m_eNowStep = LOADING_STEP::NONE;
		return E_FAIL;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	string jsonText(fileSize, '\0');
	DWORD bytesRead = 0;
	ReadFile(hFile, jsonText.data(), fileSize, &bytesRead, NULL);
	CloseHandle(hFile);

	json jScene = json::parse(jsonText);
	jLayers = jScene["layers"];
	currentLayerIter = jLayers.begin(); // 처음부터
	layerCount = jLayers.size();

	m_pFrom = from;
	m_pTo = to;
	m_pTo->Init_Layers();
	m_eNowStep = LOADING_STEP::LOAD_LAYER;

	return S_OK;
}


//우선 씬의 각 레이어에 넣어줌
void SceneLoding::Load_Layer()
{
	if (currentLayerIter == jLayers.end())
		return; 

	// 대상 씬에 레이어 얻기
	string layerName = currentLayerIter.key();
	LAYERID eID = CFactory::stringToLayer(layerName);
	m_ReadingLayer = m_pTo->Get_Layer(eID);
	if (m_ReadingLayer) 
	{
		jObjects = currentLayerIter.value()["objects"];
		ObjectsCount = jObjects.size();
		currentObjectIter = jObjects.begin();
		m_eNowStep = LOADING_STEP::LOAD_OBJ;
	}
}

void SceneLoding::LoadObject()
{
	if (currentObjectIter == jObjects.end()) {
		m_eNowStep = LOADING_STEP::LOAD_LAYER;
		++currentLayerIter;
		m_fProgress += 1.f;
		m_fProgressTimer = 0.f;
		layerCount++;
		return;
	}

	if (!currentObjectIter->is_object()) {
		++currentObjectIter;
		return;
	}

	// 하나의 오브젝트만 처리
	CGameObject* obj = CFactory::DeSerializeObject(*currentObjectIter);
	if (obj)
	{
		string nameStr = currentObjectIter->value("name", "");
		m_ReadingLayer->Add_GameObject(CFactory::ToWString(nameStr), obj);
	}

	++currentObjectIter;
	m_fProgressTimer = 0.f; 
}

void SceneLoding::Change_Scene()
{
	CSceneMgr::Get_Instance()->Set_CurrentScene(m_pTo);

	Safe_Release(m_pFrom);

	m_pTo = nullptr;
	m_pFrom = nullptr;
	layerCount = 0;
	ObjectsCount = 0;
	jLayers = nullptr;
	jObjects = nullptr;

	m_fProgressTimer = 0;
}

void SceneLoding::Free()
{
	if (m_pFrom) Safe_Release(m_pFrom);
	m_pFrom = nullptr;

	if (m_pTo) Safe_Release(m_pTo);
	m_pTo = nullptr;

	CScene::Free();
}