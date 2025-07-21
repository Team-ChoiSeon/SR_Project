#pragma once
#include "CUI.h"
BEGIN(Engine)

class CTransform;
class CTexture;

class ENGINE_DLL CUiQuad :
    public CUI
{
private:
    explicit CUiQuad(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CUiQuad();

public:
   static CUiQuad* Create(LPDIRECT3DDEVICE9 pGraphicDev);
   virtual HRESULT Ready_Component()override;

    void Update_Component(const _float& fTimeDelta) override;
    void LateUpdate_Component(const _float& fTimeDelta) override;
    void Render(LPDIRECT3DDEVICE9 pDevice) override;
    void Set_Texture(const wstring& key);
    void Set_QuadPos(_vec3 pos, _vec2 scale);

private:
    CTransform* m_pTransform = nullptr;
    CTexture* m_pTexture = nullptr;

    LPDIRECT3DDEVICE9 m_pDevice = nullptr;
    LPDIRECT3DVERTEXBUFFER9 m_pVB= nullptr;
    LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
public:
    virtual void Free()override;
};

END