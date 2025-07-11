#pragma once
#include "CBase.h"
#include "CUI.h"
#include "CUiImage.h"

BEGIN(Engine)

class ENGINE_DLL CUiMgr :public CBase
{
	DECLARE_SINGLETON(CUiMgr)

private:
	explicit CUiMgr();
	virtual  ~CUiMgr();

public:
	HRESULT Ready_UiMgr();
	void AddUI(CGameObject* pUI);
	void UpdateUI(const _float& fTimeDelta);
	void LateUpdateUI(const _float& fTimeDelta);
	void RenderUI();

private:
	vector<CGameObject*> m_vecUI;
	LPDIRECT3DDEVICE9    m_pGraphicDev = nullptr;

	D3DXMATRIX           m_matOrtho;      // UI Àü¿ë Projection
	D3DXMATRIX           m_matIdentity;   // World/View ¿ë Identity

private:
	virtual void Free() override;
};

END