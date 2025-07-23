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
    void DrawQuad(LPDIRECT3DDEVICE9 pDevice);
public:
    void Set_Texture(const wstring& key);
    void Set_Shader(const wstring& key);
    void Set_RatioX(float _ratio) { m_fRatio.x = _ratio; };
    void Set_RatioY(float _ratio) { m_fRatio.y = _ratio; };
    void Set_Ratio(_vec2 _ratio) { m_fRatio= _ratio; };
    void Set_Alpha(float alpha) { m_fAlpha = alpha; };
    void Set_TextureHandle(LPDIRECT3DBASETEXTURE9 tex);
public:
    _float Get_Alpha() { return m_fAlpha; }
private:
    CTransform* m_pTransform = nullptr;
    CTexture* m_pTexture = nullptr;
    LPDIRECT3DBASETEXTURE9 m_pTexHandle = nullptr;

    LPD3DXEFFECT m_pEffect = nullptr; // ºŒ¿Ã¥ı ∞¥√º

    LPDIRECT3DDEVICE9 m_pDevice = nullptr;
    LPDIRECT3DVERTEXBUFFER9 m_pVB= nullptr;
    LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;

    _float m_fAlpha = 1.0f;
    _vec2 m_fRatio = { 1.f ,1.f};
public:
    virtual void Free()override;
};

END