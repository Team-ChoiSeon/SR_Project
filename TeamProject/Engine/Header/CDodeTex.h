#pragma once
#include "CMesh.h"

BEGIN(Engine)

class ENGINE_DLL CDodeTex : public CMesh
{
private:
    explicit CDodeTex();
    explicit CDodeTex(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CDodeTex(const CDodeTex& rhs);

public:
    virtual ~CDodeTex();

public:
    virtual HRESULT Ready_Buffer();
    virtual void Render_Buffer();

public:
    static CDodeTex* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    virtual void Free() override;
};

END
