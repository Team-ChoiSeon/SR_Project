#pragma once
#include "Engine_Define.h"
#include "CUiMgr.h"
#include "CGameObject.h"

#include "CGraphicDev.h"

IMPLEMENT_SINGLETON(CUiMgr)

CUiMgr::CUiMgr() 
{
}

CUiMgr::~CUiMgr() 
{ 
	Free(); 
}

HRESULT CUiMgr::Ready_UiMgr()
{
	m_pGraphicDev = CGraphicDev::Get_Instance()->Get_GraphicDev();
	if (!m_pGraphicDev)
		return E_FAIL;

	D3DXMatrixIdentity(&m_matIdentity);

	D3DXMatrixOrthoLH(&m_matOrtho,
		static_cast<float>(WINCX),
		static_cast<float>(WINCY),
		0.f,   // near
		1.f);  // far

	return S_OK;
}

void CUiMgr::AddUI(CGameObject* pUI)
{
	if (pUI) {
		pUI->AddRef();
		m_vecUI.push_back(pUI);
	}
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
	if (!m_pGraphicDev)
		return;

	D3DXMATRIX matOldView, matOldProj;
	m_pGraphicDev->GetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->GetTransform(D3DTS_PROJECTION, &matOldProj);

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &m_matIdentity);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &m_matOrtho);

	for (auto& ui : m_vecUI)
		ui->Render_GameObject();

	m_pGraphicDev->SetTransform(D3DTS_VIEW, &matOldView);
	m_pGraphicDev->SetTransform(D3DTS_PROJECTION, &matOldProj);

}

void CUiMgr::Free()
{
	for (auto& ui : m_vecUI)
		Safe_Release(ui);

	m_vecUI.clear();
}