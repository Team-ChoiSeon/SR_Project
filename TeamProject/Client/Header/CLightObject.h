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
private:
	CLightObject(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLightObject();

public:
	HRESULT Ready_GameObject();
	int Update_GameObject(const _float& fTimeDelta) override;
	void LateUpdate_GameObject(const _float& fTimeDelta) override;
	void Render_GameObject() override;

	const D3DLIGHT9& GetLightInfo() ;
	CLight::LIGHTTYPE GetLightType() ;

	static CLightObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	void Free() override;

private:
	const _matrix* m_mWorld;

	float m_fIntensity;
	D3DXCOLOR m_LightColor = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);

};

