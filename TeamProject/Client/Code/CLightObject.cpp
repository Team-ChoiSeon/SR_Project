#include "pch.h"
#include "CLightObject.h"

CLightObject::CLightObject(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev), m_fIntensity(1.0f), m_LightColor(D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f))
{
	Add_Component<CTransform>(ID_DYNAMIC, pGraphicDev);
	Add_Component<CLight>(ID_DYNAMIC, pGraphicDev);
}

CLightObject::~CLightObject()
{
}

HRESULT CLightObject::Ready_GameObject()
{
    Get_Component<CTransform>()->Ready_Component();

    D3DLIGHT9 lightInfo = {};
    lightInfo.Type = D3DLIGHT_POINT;
    lightInfo.Diffuse = m_LightColor;
    lightInfo.Ambient = m_LightColor * 0.5f;
    lightInfo.Specular = m_LightColor * 0.7f;
    lightInfo.Position = Get_Component<CTransform>()->Get_Pos();
    lightInfo.Range = 10.0f;
    lightInfo.Attenuation0 = 1.0f;

    if (FAILED(Get_Component<CLight>()->Ready_Light(&lightInfo, CLight::POINT_LIGHT))) {
        return E_FAIL;
    }

    Get_Component<CLight>()->Set_LightColor(m_LightColor);

    return S_OK;
}

int CLightObject::Update_GameObject(const _float& fTimeDelta)
{
	for (auto& pComponent : m_umComponent[ID_DYNAMIC])
		pComponent.second->Update_Component(fTimeDelta);

	return CGameObject::Update_GameObject(fTimeDelta);
}

void CLightObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
	CGameObject::LateUpdate_GameObject(fTimeDelta);
}

void CLightObject::Render_GameObject()
{
	m_mWorld = Get_Component<CTransform>()->Get_WorldMatrix();
	m_pGraphicDev->SetTransform(D3DTS_WORLD, m_mWorld);
}


const D3DLIGHT9& CLightObject::GetLightInfo() 
{
    return Get_Component<CLight>()->Get_LightInfo();
}

CLight::LIGHTTYPE CLightObject::GetLightType() 
{
    return Get_Component<CLight>()->Get_LightType();
}

CLightObject* CLightObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CLightObject* pLightObject = new CLightObject(pGraphicDev);

	if (FAILED(pLightObject->Ready_GameObject()))
	{
		Safe_Release(pLightObject);
		MSG_BOX("LightObject Create Failed");
		return nullptr;
	}

	return pLightObject;
}

void CLightObject::Free()
{
    CGameObject::Free();
}
