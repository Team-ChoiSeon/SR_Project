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
    void Set_Shader(const wstring& path); // ShaderMgr ����

    CTexture* Get_Diffuse() const { return m_pDiffuse; }
    CTexture* Get_Normal() const { return m_pNormal; }
    CTexture* Get_Roughness() const { return m_pRoughness; }
    LPD3DXEFFECT Get_Effect() const { return m_pEffect; }

    wstring& Get_ShaderKey() { return m_wShaderKey; }
    wstring& Get_MatrialKey() { return m_wMatKey; }

    void Set_Material(const D3DMATERIAL9& tMtrl) { m_tMaterial = tMtrl; }
    CResource* Get_MaterialResource() const
    {
        return const_cast<CMaterial*>(this);
    }
    const D3DMATERIAL9& Get_Material() const { return m_tMaterial; }
    void Set_MaterialKey(const wstring& path) { m_wMatKey = path; }

public:
    virtual HRESULT Ready_Material();
    void Apply(LPDIRECT3DDEVICE9 pDevice); // GPU ���ε�
    virtual void Free() override;

private:
    D3DMATERIAL9 m_tMaterial;
    wstring m_wMatKey; //���̴� Ű

    CTexture* m_pDiffuse = nullptr;
    CTexture* m_pNormal = nullptr;
    CTexture* m_pRoughness = nullptr;


    LPD3DXEFFECT m_pEffect = nullptr; // ���̴� ��ü
    wstring m_wShaderKey; //���̴� Ű
};
END
