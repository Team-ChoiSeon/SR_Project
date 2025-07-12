#pragma once
#include "CComponent.h"

BEGIN(Engine)

class ENGINE_DLL CLight : public CComponent
{
public:
	enum LIGHTTYPE {DIRECTIONAL_LIGHT, POINT_LIGHT, SPOT_LIGHT};

private:
	explicit CLight();
	explicit CLight(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CLight(const CLight& rhs);

public:
	virtual ~CLight();

public:
	HRESULT			Ready_Light(const D3DLIGHT9* pLightInfo, LIGHTTYPE type);

	virtual HRESULT	Ready_Component()override { return S_OK; };
	virtual void	Update_Component(const _float& fTimeDelta)override;
	virtual void	LateUpdate_Component(const _float& fTimeDelta)override;

	void			EnableLight(_bool bEnable);

	void			Set_LightInfo(const D3DLIGHT9& lightInfo);
	void			Set_LightIndex(_uint index);
	void			Set_ViewProjectionMatrix(const D3DXMATRIX& viewProjMatrix);
	void			Set_LightType(LIGHTTYPE type);
	void			Set_LightColor(const D3DXCOLOR& color);

	const D3DLIGHT9& Get_LightInfo() const;
	const D3DXMATRIX& Get_ViewProjectionMatrix() const;
	_uint			 Get_LightIndex() const;
	LIGHTTYPE		 Get_LightType() const;
	const D3DXCOLOR& Get_LightColor() const;

private:
	void Update_ViewProjectionMatrix();

public:
	static CLight* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
	virtual void Free()override;

protected:
	D3DLIGHT9	m_tLightInfo;	// ����
	D3DXMATRIX   m_matViewProj;
	_bool		m_bEnabled;		// ��뿩��
	_uint		m_iLightIndex;	// ���� �ε���
	LIGHTTYPE    m_eLightType;	// Ÿ��
	D3DXCOLOR   m_LightColor;   // ����
};

END