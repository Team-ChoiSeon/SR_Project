#pragma once
#include "Engine_Define.h"
#include "CUiMgr.h"
#include "CGameObject.h"

IMPLEMENT_SINGLETON(CUiMgr)

CUiMgr::CUiMgr() 
{
}

CUiMgr::~CUiMgr() 
{ 
	Free(); 
}

void CUiMgr::AddUI(CGameObject* pUI)
{
	if (pUI)
		m_vecUI.push_back(pUI);
}

void CUiMgr::UpdateUI(const _float& fTimeDelta)
{
	for (auto& ui : m_vecUI)
		ui->Update_GameObject(fTimeDelta);
}

void CUiMgr::LateUpdateUI(const _float& fTimeDelta)
{
	for (auto& ui : m_vecUI)
		ui->LateUpdate_GameObject(fTimeDelta);
}

void CUiMgr::RenderUI()
{
	for (auto& ui : m_vecUI)
		ui->Render_GameObject();
}

void CUiMgr::Free()
{
	for (auto& ui : m_vecUI)
		Safe_Release(ui);
	m_vecUI.clear();
}