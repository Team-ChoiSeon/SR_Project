#include "pch.h"
#include "Engine_Define.h"
#include "CHealthPanel.h"
#include "CHealthBar.h"
#include "CCameraMgr.h"
#include "CSceneMgr.h"
#include "CUiQuad.h"
#include "CGuiSystem.h"
#include "CHealthIcon.h"
#include "CHealthLine.h"
#include "CInputMgr.h"
#include "CMainPlayer.h"
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
	m_pHealthLine = CHealthLine::Create(m_pGraphicDev);

	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);

	m_tPanel.Set_Size({ 400,100 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Left, { 0,0 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Bottom, { 0,WINCY });

	return S_OK;
}

_int CHealthPanel::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_pPlayer) {
		if (CGameObject* obj =  CSceneMgr::Get_Instance()->Get_Player()) {
			m_pPlayer = static_cast<CMainPlayer*>(obj);
		}
	}

	CGameObject::Update_GameObject(fTimeDelta);

	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
	_vec2 tmp = m_tPanel.LC() + _vec2(20, 0);
	m_pHealthIcon->Set_Pivot(tmp);

	tmp += _vec2(35, 0);
	m_pHealthBar->Set_Pivot(tmp);

	m_pHealthLine->Set_Pivot(m_pHealthBar->Get_Panel().LC());
	m_pHealthBar->Update_GameObject(fTimeDelta);
	m_pHealthIcon->Update_GameObject(fTimeDelta);
	m_pHealthLine->Update_GameObject(fTimeDelta);


	if(CInputMgr::Get_Instance()->Key_Tap(DIK_I)){
		tmpHealth -= 0.1f;
		m_pHealthBar->Set_Ratio(tmpHealth);
	}

	return 0;
}

void CHealthPanel::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
	m_pHealthIcon->LateUpdate_GameObject(fTimeDelta);
	m_pHealthBar->LateUpdate_GameObject(fTimeDelta);
	m_pHealthLine->LateUpdate_GameObject(fTimeDelta);
}

void CHealthPanel::Free()
{
	CGameObject::Free();
	Safe_Release(m_pHealthIcon);
	Safe_Release(m_pHealthBar);
	Safe_Release(m_pHealthLine);
	Safe_Release(m_pGraphicDev);
}
