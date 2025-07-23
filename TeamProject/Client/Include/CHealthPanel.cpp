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
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pQuad->Set_Texture(L"UI/CinematicBar.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");
	m_pQuad->Set_Alpha(0.4);
	m_tPanel.Set_Size({ 220,50 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Left, { -10,0 });
	m_tPanel.Set_Anchor(UIPanel::Anchor::Bottom, { 0,WINCY-10 });
	m_pTransform->Rotate_Axis({ 0,0,1 }, D3DXToRadian(3.f));

	return S_OK;
}

_int CHealthPanel::Update_GameObject(const _float& fTimeDelta)
{
	if (!m_pPlayer) {
		if (CGameObject* obj =  CSceneMgr::Get_Instance()->Get_Player()) {
			m_pPlayer = static_cast<CMainPlayer*>(obj);
		}
	}
	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });

	CGameObject::Update_GameObject(fTimeDelta);

	_vec2 tmp = m_tPanel.LC() + _vec2(16,6);
	m_pHealthIcon->Set_Pivot(tmp);

	tmp = m_tPanel.LC()+_vec2(55, 0);
	m_pHealthBar->Set_Pivot(tmp);
	m_pHealthLine->Set_Pivot(m_pHealthBar->Get_Panel().LC());

	m_pHealthBar->Update_GameObject(fTimeDelta);
	m_pHealthIcon->Update_GameObject(fTimeDelta);
	m_pHealthLine->Update_GameObject(fTimeDelta);
	m_pHealthBar->Set_Ratio(m_pPlayer->Get_Hp());


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
