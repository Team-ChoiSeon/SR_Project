#include "pch.h"
#include "CHealthLine.h"
#include "CUiQuad.h"
#include "CTransform.h"

CHealthLine::CHealthLine(LPDIRECT3DDEVICE9 pGraphicDev)
    :CGameObject(pGraphicDev)
{
}

CHealthLine::~CHealthLine()
{
}

CHealthLine* CHealthLine::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthLine* instance = new CHealthLine(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CHealthLine::Ready_GameObject()
{
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pQuad->Set_Texture(L"UI/TXUI_healthbar_line_overlay.png");

	m_tPanel.Set_Size({ 150,20 });
	m_pTransform->Rotate_Axis({ 0,0,1 }, D3DXToRadian(3.f));

	return S_OK;
}

_int CHealthLine::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });

	return 0;
}

void CHealthLine::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHealthLine::Set_Pivot(_vec2 vCriteria)
{
	m_tPanel.Set_Anchor(UIPanel::Anchor::LeftCenter, vCriteria);
}

void CHealthLine::Free()
{
    CGameObject::Free();
    Safe_Release(m_pGraphicDev);
}
