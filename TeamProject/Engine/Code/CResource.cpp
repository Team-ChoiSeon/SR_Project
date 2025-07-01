#include "CResource.h"

CResource::CResource()
	: m_pGraphicDev(nullptr)
{
}

CResource::CResource(LPDIRECT3DDEVICE9 pGraphicDev)
	: m_pGraphicDev(pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CResource::CResource(const CResource& rhs)
	: m_pGraphicDev(rhs.m_pGraphicDev)
{
	m_pGraphicDev->AddRef();
}

CResource::~CResource()
{
}

void CResource::Free()
{
	Safe_Release(m_pGraphicDev);
}
