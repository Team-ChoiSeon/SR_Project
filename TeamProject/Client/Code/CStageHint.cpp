#include "pch.h"
#include "CStageHint.h"
#include"Engine_Define.h"
#include "CHints.h"

CStageHint::CStageHint(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CStageHint::~CStageHint()
{
}

CStageHint* CStageHint::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CStageHint* instance = new CStageHint(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;	
	}

	return instance;
}

HRESULT CStageHint::Ready_GameObject()
{
	m_vecHint.reserve(8);

	for (int i = 0; i < 8; ++i) {
		m_vecHint.push_back(CHints::Create(m_pGraphicDev));
	}

	return S_OK;
}

_int CStageHint::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& hints : m_vecHint) {
		if (hints->Get_Active())
			hints->Update_GameObject(fTimeDelta);
	}
	return 0;
}

void CStageHint::LateUpdate_GameObject(const _float& fTimeDelta)
{
	for (auto& hints : m_vecHint) {
		if (hints->Get_Active())
			hints->LateUpdate_GameObject(fTimeDelta);
	}
}

void CStageHint::Add_Hint(const wstring& hint)
{
	for (auto& hints : m_vecHint) {
		if (!hints->Get_Active()) {
			 hints->Set_Active(hint);
			 return;
		}
	}

	CHints* hintObj = CHints::Create(m_pGraphicDev);
	hintObj->Set_Active(hint);
	m_vecHint.push_back(hintObj);
}

void CStageHint::Free()
{
	for (auto& hints : m_vecHint) {
		Safe_Release(hints);
	}
}
