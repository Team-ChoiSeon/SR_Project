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
	void AddLight(CLight* pLight);
	void UpdateLights(const D3DXVECTOR3& cameraPos);

private:
	std::list<CLight*> m_LightList;


private:
	virtual void Free() override;
};

END