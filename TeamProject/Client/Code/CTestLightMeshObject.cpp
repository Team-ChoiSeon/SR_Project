#include "pch.h"
#include "CTestLightMeshObject.h"
#include "CLight.h"
#include "CLightMgr.h"
#include "CTransform.h"

CTestLightMeshObject::CTestLightMeshObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
    Add_Component<CTransform>(ID_DYNAMIC, pGraphicDev);
}

CTestLightMeshObject::~CTestLightMeshObject()
{
    Free();
}

HRESULT CTestLightMeshObject::Ready_GameObject()
{
    if (FAILED(D3DXCreateSphere(m_pGraphicDev, 0.3f, 20, 20, &m_pMesh, nullptr)))
        return E_FAIL;

    ZeroMemory(&m_Material, sizeof(D3DMATERIAL9));
    m_Material.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    m_Material.Specular = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    m_Material.Ambient = D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.f);
    m_Material.Emissive = D3DXCOLOR(0.f, 0.f, 0.f, 1.f);

    Get_Component<CTransform>()->Ready_Transform();
    Get_Component<CTransform>()->Set_Pos({ 0.f, 0.5f, 0.f });

    CLight* pLight = CLight::Create(m_pGraphicDev);
    if (!pLight) return E_FAIL;

    D3DLIGHT9 light = {};
    light.Type = D3DLIGHT_POINT;
    light.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);
    light.Ambient = D3DXCOLOR(0.4f, 0.4f, 0.4f, 1.f);
    light.Specular = D3DXCOLOR(0.8f, 0.8f, 0.8f, 1.f);
    light.Position = D3DXVECTOR3(0.f, 0.5f, 2.f);
    light.Range = 20.f;
    light.Attenuation0 = 1.f;

    pLight->Ready_Light(&light, CLight::POINT_LIGHT);
    pLight->Set_LightIndex(0);
    pLight->EnableLight(TRUE);

    Engine::CLightMgr::Get_Instance()->AddLight(pLight);

    return S_OK;
}

_int CTestLightMeshObject::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& comp : m_umComponent[ID_DYNAMIC])
        comp.second->Update_Component(fTimeDelta);
    return 0;
}

void CTestLightMeshObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& comp : m_umComponent[ID_DYNAMIC])
        comp.second->LateUpdate_Component();
}

void CTestLightMeshObject::Render_GameObject()
{
    m_pGraphicDev->SetRenderState(D3DRS_LIGHTING, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_SPECULARENABLE, TRUE);
    m_pGraphicDev->SetRenderState(D3DRS_ZENABLE, TRUE);

    const _matrix* matWorld = Get_Component<CTransform>()->Get_WorldMatrix();
    m_pGraphicDev->SetTransform(D3DTS_WORLD, matWorld);
    m_pGraphicDev->SetMaterial(&m_Material);

    if (m_pMesh)
        m_pMesh->DrawSubset(0);
}

void CTestLightMeshObject::Free()
{
    Safe_Release(m_pMesh);

    CGameObject::Free();
}
