#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CCameraCom : public CComponent
{
private:
	explicit CCameraCom();
	explicit CCameraCom(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CCameraCom(const CCameraCom& rhs);
	virtual ~CCameraCom();

public:
	HRESULT		Ready_Camera();
	virtual void Update_Camera(const _float& fTimeDelta);
	virtual void LateUpdate_Camera();

public:			//GetSet
};

END