#include "CLight.h"

CLight::CLight()
    : m_bEnabled(false), m_iLightIndex(0), m_eLightType(DIRECTIONAL_LIGHT), m_LightColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
{
    ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
    D3DXMatrixIdentity(&m_matViewProj);
}

CLight::CLight(LPDIRECT3DDEVICE9 pGraphicDev)
    : CComponent(pGraphicDev), m_bEnabled(false), m_iLightIndex(0), m_eLightType(DIRECTIONAL_LIGHT), m_LightColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
{
    ZeroMemory(&m_tLightInfo, sizeof(D3DLIGHT9));
    D3DXMatrixIdentity(&m_matViewProj);
}

CLight::CLight(const CLight& rhs)
    : CComponent(rhs), m_tLightInfo(rhs.m_tLightInfo), m_matViewProj(rhs.m_matViewProj),
    m_bEnabled(rhs.m_bEnabled), m_iLightIndex(rhs.m_iLightIndex), m_eLightType(rhs.m_eLightType), m_LightColor(rhs.m_LightColor)
{
}

CLight::~CLight()
{
    Free();
}

HRESULT CLight::Ready_Light(const D3DLIGHT9* pLightInfo, LIGHTTYPE type)
{
    memcpy(&m_tLightInfo, pLightInfo, sizeof(D3DLIGHT9));
    m_eLightType = type;
    // Update_ViewProjectionMatrix();
    
    // if (m_eLightType == DIRECTIONAL_LIGHT) {
    //     // D3DXVec3Normalize(&m_tLightInfo.Direction, &m_tLightInfo.Direction);
    // }
    // else if (m_eLightType == POINT_LIGHT || m_eLightType == SPOT_LIGHT) {
    //     m_tLightInfo.Position = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
    // }
    return S_OK;
}

void CLight::Update_Component(const _float& fTimeDelta)
{
    Update_ViewProjectionMatrix();
}

void CLight::LateUpdate_Component()
{
    if (!m_pGraphicDev)
        return;
    m_pGraphicDev->SetLight(m_iLightIndex, &m_tLightInfo);
    m_pGraphicDev->LightEnable(m_iLightIndex, m_bEnabled);

    // if (m_bEnabled)
    // {
    //     m_pGraphicDev->SetLight(m_iLightIndex, &m_tLightInfo);
    //     m_pGraphicDev->LightEnable(m_iLightIndex, TRUE);
    // }
    // else
    // {
    //     m_pGraphicDev->LightEnable(m_iLightIndex, FALSE);
    // }
}

void CLight::EnableLight(_bool bEnable)
{
    m_bEnabled = bEnable;
}

void CLight::Set_LightInfo(const D3DLIGHT9& lightInfo)
{
    m_tLightInfo = lightInfo;
    Update_ViewProjectionMatrix();
}

void CLight::Set_LightIndex(_uint index)
{
    m_iLightIndex = index;
}

void CLight::Set_ViewProjectionMatrix(const D3DXMATRIX& viewProjMatrix)
{
    m_matViewProj = viewProjMatrix;
}

void CLight::Set_LightType(LIGHTTYPE type)
{
    m_eLightType = type;
}

void CLight::Set_LightColor(const D3DXCOLOR& color)
{
    m_LightColor = color;
    m_tLightInfo.Diffuse = color;
    m_tLightInfo.Ambient = color * 0.5f;
    m_tLightInfo.Specular = color * 0.7f;
}

const D3DLIGHT9& CLight::Get_LightInfo() const
{
    return m_tLightInfo;
}

const D3DXMATRIX& CLight::Get_ViewProjectionMatrix() const
{
    return m_matViewProj;
}

_uint CLight::Get_LightIndex() const
{
    return m_iLightIndex;
}

CLight::LIGHTTYPE CLight::Get_LightType() const
{
    return m_eLightType;
}

const D3DXCOLOR& CLight::Get_LightColor() const
{
    return m_LightColor;
}

CLight* CLight::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLight* pLight = new CLight(pGraphicDev);

    // if (FAILED(pLight->Ready_Light()))
    // {
    //     Safe_Release(pLight);
    //     MSG_BOX("CubeTex Create Failed");
    //     return nullptr;
    // }

    return pLight;
}

void CLight::Update_ViewProjectionMatrix()
{
    if (m_eLightType != SPOT_LIGHT && m_eLightType != DIRECTIONAL_LIGHT)
        return;

    D3DXVECTOR3 vEye = m_tLightInfo.Position;
    D3DXVECTOR3 vAt = vEye + m_tLightInfo.Direction;
    D3DXVECTOR3 vUp = D3DXVECTOR3(0.f, 1.f, 0.f);

    D3DXMATRIX matView, matProj;
    D3DXMatrixLookAtLH(&matView, &vEye, &vAt, &vUp);
    D3DXMatrixPerspectiveFovLH(&matProj, D3DXToRadian(90.f), 1.f, 1.f, 100.f);

    m_matViewProj = matView * matProj;
}


void CLight::Free()
{
}
