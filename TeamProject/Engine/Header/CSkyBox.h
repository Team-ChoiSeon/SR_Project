#pragma once
#include "CComponent.h"
BEGIN(Engine)
class CTransform;
class ENGINE_DLL CSkyBox :
    public CComponent
{
private:
    explicit CSkyBox(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CSkyBox();
public:
    static CSkyBox* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    HRESULT Ready_Component() override;
    void Update_Component(const _float& fTimeDelta) override;
    void LateUpdate_Component(const _float& fTimeDelta) override;
    void Render(LPDIRECT3DDEVICE9 pDevice);
public:
    void Set_Texture(const wstring& key);
private:
    CTransform* m_pTransform = nullptr;
    LPDIRECT3DCUBETEXTURE9 m_pCubeTexture = nullptr;
    LPDIRECT3DVERTEXBUFFER9 m_pVB = nullptr;
    LPDIRECT3DINDEXBUFFER9 m_pIB = nullptr;
private:
    void Free() override;
};
END
