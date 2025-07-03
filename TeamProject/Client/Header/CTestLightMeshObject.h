#pragma once
#include "CGameObject.h"

class CTestLightMeshObject : public Engine::CGameObject
{
public:
    explicit CTestLightMeshObject(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CTestLightMeshObject();

    HRESULT Ready_GameObject() override;
    _int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject(const _float& fTimeDelta) override;
    void Render_GameObject() override;

private:
    LPD3DXMESH m_pMesh = nullptr;
    D3DMATERIAL9 m_Material;

private:
    void Free() override;
};
