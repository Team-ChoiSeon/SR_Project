#include "pch.h"
#include "CHealthBar.h"
#include "CUiQuad.h"
#include "CTransform.h"
#include "CResourceMgr.h"


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
	m_pHealthBar = Add_Component<CUiQuad>(ID_DYNAMIC,m_pGraphicDev);
	m_pTransform= Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	m_pHealthBar->Set_Texture(L"UI/bar_health_full_straight.png");
	m_pTransform->Set_Scale({ .4f,.05f,1 });
	return S_OK;
}

_int CHealthBar::Update_GameObject(const _float& fTimeDelta)
{
	CGameObject::Update_GameObject(fTimeDelta);
	return 0;
}

void CHealthBar::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CHealthBar::Set_Pos(_vec3 vCriteria, _vec3 vPos)
{
	m_pTransform->Set_Pos(vCriteria + vPos);
}

void CHealthBar::Free()
{
	CGameObject::Free();
	Safe_Release(m_pGraphicDev);
}
