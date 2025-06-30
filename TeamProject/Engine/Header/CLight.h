#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CComponent
{
private:
	explicit CLight();
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLight(const CLight& rhs);
	virtual ~CLight();

public:
	HRESULT			Ready_Light(const D3DLIGHT9* pLightInfo);
	virtual void	Update_Component(const _float& fTimeDelta);
	virtual void	LateUpdate_Component();

	void			EnableLight(_bool bEnable);

	void			SetLightInfo(const D3DLIGHT9& lightInfo);
	void			SetLightIndex(_uint index);

	const D3DLIGHT9& GetLightInfo() const;
	_uint			GetLightIndex() const;

private:
	virtual void Free();

protected:
	D3DLIGHT9	m_tLightInfo;	// ����
	_bool		m_bEnabled;		// ��뿩��
	_uint		m_iLightindex;	// ���� �ε���
};

END