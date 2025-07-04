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
	HRESULT Ready_Image(CTexture* pTex);
	virtual void Update_Component(const _float& fTimeDelta) override;
	virtual void LateUpdate_Component() override;
	virtual void Render_Component() override;

	void Set_Texture(CTexture* pTex) { m_pTexture = pTex; }
	CTexture* Get_Texture() const { return m_pTexture; }

	static CUiImage* Create(LPDIRECT3DDEVICE9 pGraphicDev);

protected:
	CTexture* m_pTexture = nullptr;

protected:
	virtual void Free() override;

};

END

