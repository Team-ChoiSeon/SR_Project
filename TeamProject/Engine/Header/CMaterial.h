#pragma once
#include "CResource.h"
#include "CTexture.h"

BEGIN(Engine)

class ENGINE_DLL CMaterial : public CResource
{
public:
    explicit CMaterial();
    virtual ~CMaterial();

public:
    static  CMaterial* Create();

public:
    void Set_Diffuse(CTexture* tex)   { m_pDiffuse = tex; }
    void Set_Normal(CTexture* tex)    { m_pNormal = tex; }
    void Set_Roughness(CTexture* tex) { m_pRoughness = tex; }

    CTexture* Get_Diffuse() const { return m_pDiffuse; }
    CTexture* Get_Normal() const { return m_pNormal; }
    CTexture* Get_Roughness() const { return m_pRoughness; }

    void Set_Material(const D3DMATERIAL9& tMtrl) { m_tMaterial = tMtrl; }
    CResource* Get_MaterialResource() const
    {
        return const_cast<CMaterial*>(this);
    }
    const D3DMATERIAL9& Get_Material() const { return m_tMaterial; }

public:
    virtual HRESULT Ready_Material();
    void Apply(LPDIRECT3DDEVICE9 pDevice); // GPU ¹ÙÀÎµù
    virtual void Free() override;

private:
    D3DMATERIAL9 m_tMaterial;
    CTexture* m_pDiffuse = nullptr;
    CTexture* m_pNormal = nullptr;
    CTexture* m_pRoughness = nullptr;
};
END
