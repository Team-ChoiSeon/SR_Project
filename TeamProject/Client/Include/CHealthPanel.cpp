#include "pch.h"
#include "Engine_Define.h"
#include "CHealthPanel.h"
#include "CHealthBar.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CUiQuad.h"
#include "CGuiSystem.h"
#include "CHealthIcon.h"

CHealthPanel::CHealthPanel(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CHealthPanel::~CHealthPanel()
{
}

CHealthPanel* CHealthPanel::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthPanel* instance = new CHealthPanel(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CHealthPanel::Ready_GameObject()
{
	m_pHealthBar = CHealthBar::Create(m_pGraphicDev);
	m_pHealthIcon = CHealthIcon::Create(m_pGraphicDev);

	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);

	m_pQuad->Set_Texture(L"UI/Vinjette.png");

	m_tPanel.Set_Size({ 400,100 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Left, { 0,0 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Bottom, { 0,WINCY });

	return S_OK;
}

_int CHealthPanel::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);

	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });

	m_pHealthBar->Set_Pivot(m_tPanel.Get_Pos());

	_vec2 tmp =m_tPanel.Get_Pos() - _vec2(40, 0);
	m_pHealthIcon->Set_Pivot(tmp);

	m_pHealthBar->Update_GameObject(fTimeDelta);
	m_pHealthIcon->Update_GameObject(fTimeDelta);
	return 0;
}

void CHealthPanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pHealthIcon->LateUpdate_GameObject(fTimeDelta);
	m_pHealthBar->LateUpdate_GameObject(fTimeDelta);
}

void CHealthPanel::Free()
{
	CGameObject::Free();
	Safe_Release(m_pHealthIcon);
	Safe_Release(m_pHealthBar);
	Safe_Release(m_pGraphicDev);
}
