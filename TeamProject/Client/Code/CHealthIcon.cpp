#include "pch.h"
#include "Engine_Define.h"
#include "CHealthIcon.h"
#include "CUiQuad.h"
#include "CTransform.h"

CHealthIcon::CHealthIcon(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CHealthIcon::~CHealthIcon()
{
}

CHealthIcon* CHealthIcon::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CHealthIcon* instance = new CHealthIcon(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CHealthIcon::Ready_GameObject()
{
	m_pQuad = Add_Component<CUiQuad>(ID_DYNAMIC, m_pGraphicDev);
	m_pTransform = Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pQuad->Set_Texture(L"UI/icon_health_straight.png");

	m_tPanel.Set_Size({ 30,30 });
	return S_OK;
}

_int CHealthIcon::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	m_pTransform->Set_Scale(m_tPanel.Get_WorldScale());
	m_pTransform->Set_Pos({ m_tPanel.Get_WorldPos(WINCX, WINCY) });

	return 0;
}

void CHealthIcon::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHealthIcon::Set_Pivot(_vec2 vCriteria)
{
	m_tPanel.Set_Anchor(UIPanel::Anchor::LeftCenter, vCriteria);
	m_tPanel.RT();
}

void CHealthIcon::Free()
{
	CGameObject::Free();
	Safe_Release(m_pGraphicDev);
}
