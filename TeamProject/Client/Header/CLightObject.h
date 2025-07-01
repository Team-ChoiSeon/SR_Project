#pragma once
#include "CGameObject.h"
#include "CLight.h"
#include "CTransform.h"

namespace Engine
{
	//class CLight;
	class CTransform;
}

class CLightObject : public Engine::CGameObject
{
public:
	CLightObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightObject();

	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;

public:		// GetSet
	const D3DLIGHT9& GetLightInfo() const { return m_pLightCom->Get_LightInfo(); }
	CLight::LIGHTTYPE GetLightType() const { return m_pLightCom->Get_LightType(); }


private:
	CLight* m_pLightCom;
	CTransform* m_pTransformCom;

	const _matrix* m_mWorld;

	D3DLIGHT9 m_tLightInfo;
	CLight::LIGHTTYPE m_eLightType;

	float m_fIntensity;
	D3DXCOLOR m_LightColor;

};

