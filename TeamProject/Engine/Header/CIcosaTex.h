#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CIcosaTex : public CMesh
{
private:
    explicit CIcosaTex();
    explicit CIcosaTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CIcosaTex(const CIcosaTex& rhs);

public:
    virtual ~CIcosaTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void Render_Buffer();

public:
    static CIcosaTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;
};

END
