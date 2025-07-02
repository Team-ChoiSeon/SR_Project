#pragma once
#include "pch.h"
#include "SceneBG.h"

SceneBG::SceneBG(LPDIRECT3DDEVICE9 pGraphicDev)
	:CScene(pGraphicDev)
{

}

SceneBG::~SceneBG()
{
}

HRESULT SceneBG::Ready_Scene()
{
	Init_Layers();
	return S_OK;
}

_int SceneBG::Update_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->Update_Layer(fTimeDelta);

	return 0;
}

void SceneBG::LateUpdate_Scene(const _float& fTimeDelta)
{
	for (auto& pLayer : m_umLayer)
		pLayer.second->LateUpdate_Layer(fTimeDelta);
}

void SceneBG::Render_Scene()
{
}

SceneBG* SceneBG::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	SceneBG* pScene = new SceneBG(pGraphicDev);

	if (FAILED(pScene->Ready_Scene()))
	{
		Safe_Release(pScene);
		MSG_BOX("SceneHW Create Failed");
		return nullptr;
	}

	return pScene;
}

void SceneBG::Free()
{
	CScene::Free();
}