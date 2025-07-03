#include "CPickTarget.h"

CPickTarget::CPickTarget()
{
}

CPickTarget::CPickTarget(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype)
	: m_HWND(hwnd), m_eRayCheckType(boundtype)
{
}

CPickTarget::CPickTarget(const CPickTarget& rhs)
{
}

CPickTarget::~CPickTarget()
{
}

HRESULT CPickTarget::Ready_Component()
{
	return S_OK;
}

void CPickTarget::Update_Component(const float& fTimeDelta)
{
}

void CPickTarget::LateUpdate_Component(const float& fTimeDelta)
{
}

CPickTarget* CPickTarget::Create(HWND hwnd, LPDIRECT3DDEVICE9 pGraphicDev, RAYCHECKTYPE boundtype)
{
	CPickTarget* pPickCom = new CPickTarget(hwnd, pGraphicDev, boundtype);

	if (FAILED(pPickCom->Ready_Component()))
	{
		Safe_Release(pPickCom);
		MSG_BOX("PickTraget Component Create Failed");
		return nullptr;
	}

	return pPickCom;
}

void CPickTarget::Free()
{
	Safe_Release(m_pGraphicDev);
}

void CPickTarget::Check_Collision()
{
}

void CPickTarget::Add_HitInfo()
{
}
