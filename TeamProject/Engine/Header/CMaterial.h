#pragma once
#include "CResource.h"
BEGIN(Engine)
class ENGINE_DLL CMaterial : public CResource
{
public:
    explicit CMaterial();
    virtual ~CMaterial();

public:
    void Set_Material(const D3DMATERIAL9& tMtrl) { m_tMaterial = tMtrl; }
    CResource* Get_MaterialResource() const
    {
        return const_cast<CMaterial*>(this);
    }
    const D3DMATERIAL9& Get_Material() const { return m_tMaterial; }

public:
    virtual HRESULT Ready_Material();
    virtual void Free() override;

private:
    D3DMATERIAL9 m_tMaterial;
};
END
