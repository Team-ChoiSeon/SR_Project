#include "CLight.h"

CLight::CLight()
	:m_bEnabled(false), m_iLightindex(0)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
}

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
	:CComponent(pGraphicDev), m_bEnabled(false), m_iLightindex(0)
{
	ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
}

CLight::CLight(const CLight& rhs)
	:CComponent(rhs), m_tLightInfo(rhs.m_tLightInfo), m_bEnabled(rhs.m_bEnabled), m_iLightindex(rhs.m_iLightindex)
{

}

CLight::~CLight()
{

}

HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo)
{
	memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));

	return S_OK;
}

void CLight::Update_Component(const _float& fTimeDelta)
{

}

void CLight::LateUpdate_Component()
{
	if (m_bEnabled) 
	{
		m_pGraphicDev->SetLight(m_iLightindex, &m_tLightInfo);
		m_pGraphicDev->LightEnable(m_iLightindex, TRUE);
	}
	else
	{
		m_pGraphicDev->LightEnable(m_iLightindex, FALSE);
	}
}

void CLight::EnableLight(_bool bEnable)
{
	m_bEnabled = bEnable;
}

void CLight::SetLightInfo(const D3DLIGHT9& lightInfo)
{
	m_tLightInfo = lightInfo;
}

void CLight::SetLightIndex(_uint index)
{
	m_iLightindex = index;
}

const D3DLIGHT9& CLight::GetLightInfo() const
{
	return m_tLightInfo;
}

_uint CLight::GetLightIndex() const
{
	return m_iLightindex;
}

void CLight::Free()
{
}