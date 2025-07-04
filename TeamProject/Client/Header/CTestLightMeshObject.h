#pragma once
#include "CGameObject.h"

class CTestLightMeshObject : public Engine::CGameObject
{
private:
    explicit CTestLightMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CTestLightMeshObject();

public:
    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject(const _float& fTimeDelta) override;
    void Render_GameObject() override;

    static CTestLightMeshObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    void Free() override;

private:
    LPD3DXMESH m_pMesh = nullptr;
    D3DMATERIAL9 m_Material;

};
