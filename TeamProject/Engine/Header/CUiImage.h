#pragma once
#include "CUI.h"
#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CUiImage : public CUI
{
private:
	explicit CUiImage();
	explicit CUiImage(LPDIRECT3DDEVICE9 pGraphicDev);
	explicit CUiImage(const CUiImage& rhs);

public:
	virtual ~CUiImage();

public:
	HRESULT Ready_Image();
	virtual void Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual void Render(LPDIRECT3DDEVICE9 pDevice) override;

	void Set_Texture(CTexture* pTex);
	LPDIRECT3DTEXTURE9 Get_Texture() const;

	static CUiImage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	
	LPDIRECT3DTEXTURE9 m_Texture = nullptr;

protected:
	virtual void Free() override;

};

END

