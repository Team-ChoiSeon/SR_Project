#include "pch.h"
#include "CHealthBar.h"
#include "CUiQuad.h"
#include "CTransform.h"


CHealthBar::CHealthBar(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CHealthBar::~CHealthBar()
{
}

CHealthBar* CHealthBar::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthBar* instance = new CHealthBar(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CHealthBar::Ready_GameObject()
{
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC,m_pGraphicDev);
	m_pTransform= Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pQuad->Set_Texture(L"UI/bar_health_full_straight.png");
	m_pQuad->Set_Shader(L"g_UIShader.fx");
	m_tPanel.Set_Size({ 150,20 });
	return S_OK;
}

_int CHealthBar::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });
	return 0;
}

void CHealthBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHealthBar::Set_Pivot(_vec2 vCriteria)
{
	m_tPanel.Set_Anchor(UIPanel::Anchor::LeftCenter, vCriteria);
}

void CHealthBar::Set_Ratio(_float ratio)
{
	m_pQuad->Set_RatioX(ratio);
}

void CHealthBar::Free()
{
	CGameObject::Free();
	Safe_Release(m_pGraphicDev);
}
