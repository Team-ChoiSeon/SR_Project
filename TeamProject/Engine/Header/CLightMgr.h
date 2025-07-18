#pragma once
#include "CLight.h"
#include "CBase.h"

BEGIN(Engine)

class ENGINE_DLL CLightMgr:public CBase
{
	DECLARE_SINGLETON(CLightMgr)

private:
	explicit CLightMgr();
	virtual  ~CLightMgr();

public:
	HRESULT Ready_Light(LPDIRECT3DDEVICE9 pDevice);
	void AddLight(CLight* pLight);
	void UpdateLights(const _float& fTimeDelta);
	D3DLIGHT9& Get_MainLight() { return m_BaseLight; }

private:
	LPDIRECT3DDEVICE9 m_pDevice;
	D3DLIGHT9 m_BaseLight;
	std::list<CLight*> m_LightList;
	float fAngle;

private:
	virtual void Free() override;
};

END