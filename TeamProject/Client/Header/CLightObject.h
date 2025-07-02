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
	const D3DLIGHT9& GetLightInfo() ;
	CLight::LIGHTTYPE GetLightType() ;


private:

	const _matrix* m_mWorld;


	float m_fIntensity;
	D3DXCOLOR m_LightColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

};

