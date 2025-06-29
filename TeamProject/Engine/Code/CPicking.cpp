#include "CPicking.h"

CPicking::CPicking()
{
}

CPicking::CPicking(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev)
    :CComponent(pGraphicDev), m_HWND(hwnd)
{
}

CPicking::CPicking(const CPicking& rhs)
    : CComponent(rhs)
{
}

CPicking::~CPicking()
{
}

HRESULT CPicking::Ready_Picking()
{
    return E_NOTIMPL;
}

void CPicking::Update_Picking(const float& fTimeDelta)
{
	//POINT cursor;
	//GetCursorPos(&cursor);
}

void CPicking::LateUpdate_Picking()
{
}

CPicking* CPicking::Create(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev)
{
	CPicking* pPicking = new CPicking(hwnd, pGraphicDev);

	if (FAILED(pPicking->Ready_Picking()))
	{
		Safe_Release(pPicking);
		MSG_BOX("Picking Create Failed");
		return nullptr;
	}

	return pPicking;
}

CComponent* CPicking::Clone()
{
    return new CPicking(*this);
}



void CPicking::Free()
{
}

void CPicking::ComputeRay()
{
}
