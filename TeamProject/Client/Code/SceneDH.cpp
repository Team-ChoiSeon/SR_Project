#pragma once
#include "pch.h"
#include "SceneDH.h"

SceneDH::SceneDH(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

SceneDH::~SceneDH()
{
}

HRESULT SceneDH::Ready_Scene()
{
	Init_Layers();
	return S_OK;
}

_int SceneDH::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	return 0;
}

void SceneDH::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
}


SceneDH* SceneDH::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneDH* pScene = new SceneDH(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}


void SceneDH::Free()
{
	CScene::Free();
}