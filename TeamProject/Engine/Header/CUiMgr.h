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
	void AddUI(CGameObject* pUI);
	void UpdateUI(const _float& fTimeDelta);
	void LateUpdateUI(const _float& fTimeDelta);
	void RenderUI();

private:
	std::vector<CGameObject*> m_vecUI;

private:
	virtual void Free() override;
};

END