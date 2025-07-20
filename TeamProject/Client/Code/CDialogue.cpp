#include "pch.h"
#include "Engine_Define.h"
#include "CDialogue.h"


CDialogue::CDialogue(LPDIRECT3DDEVICE9 pGraphicDev)
	:CGameObject(pGraphicDev)
{
}

CDialogue::~CDialogue()
{
}

CDialogue* CDialogue::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CDialogue* instance = new CDialogue(pGraphicDev);

	if (FAILED(instance->Ready_GameObject())) {
		Safe_Release(instance);
		instance = nullptr;
	}

	return instance;
}

HRESULT CDialogue::Ready_GameObject()
{
	return S_OK;
}

_int CDialogue::Update_GameObject(const _float& fTimeDelta)
{
	return _int();
}

void CDialogue::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CDialogue::Free()
{
}
