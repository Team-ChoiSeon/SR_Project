#include "pch.h"
#include "CLightObject.h"
#include "CTransform.h"

CLightObject::CLightObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : Engine::CGameObject(pGraphicDev), m_pTransformCom(nullptr), m_pLightCom(nullptr)
{
}

CLightObject::CLightObject(const CLightObject& rhs)
    : Engine::CGameObject(rhs), m_pTransformCom(nullptr), m_pLightCom(nullptr)
{
}

CLightObject::~CLightObject()
{
    Free();
}

HRESULT CLightObject::Ready_GameObject()
{
    if (FAILED(Add_Component()))
        return E_FAIL;

    return S_OK;
}

_int CLightObject::Update_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::Update_GameObject(fTimeDelta);

    if (m_pTransformCom)
        m_pTransformCom->Update_Component(fTimeDelta);

    return 0;
}

void CLightObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    Engine::CGameObject::LateUpdate_GameObject(fTimeDelta);

    //if (m_pLightCom)
    //    m_pLightCom->LateUpdate_Component();
}

void CLightObject::Render_GameObject()
{
    if (m_pGraphicDev && m_pTransformCom)
    {
        m_pGraphicDev->SetTransform(D3DTS_WORLD, m_pTransformCom->Get_WorldMatrix());
    }
}

void CLightObject::Set_Light(const D3DLIGHT9& tLightData)
{
    // if (m_pLightCom)
    //     m_pLightCom->SetLightInfo(tLightData);
}

Engine::CLight* CLightObject::Get_LightComponent()
{
    return m_pLightCom;
}

HRESULT CLightObject::Add_Component()
{
    // CComponent* pComponent = nullptr;
    // Add_Component<CTransform>()


     
    // pComponent = m_pTransformCom = dynamic_cast<Engine::CTransform*>(CProtoMgr::GetInstance()->Clone_Prototype(L"Proto_Transform"));
    // if (nullptr == pComponent)
    //     return E_FAIL;
    // m_mapComponent[ID_DYNAMIC].insert({ L"Com_Transform", pComponent });

    // m_pTransformCom = new Engine::CTransform(m_pGraphicDev);
    // if (!m_pTransformCom || FAILED(m_pTransformCom->Ready_Transform()))
    // {
    //     MSG_BOX("Failed to create Transform Component");
    //     return E_FAIL;
    // }
    // m_umComponent[ID_DYNAMIC].insert({ L"Com_Transform", m_pTransformCom });

    // m_pLightCom = new Engine::CLight(m_pGraphicDev);
    // if (!m_pLightCom || FAILED(m_pLightCom->Ready_Light(nullptr)))
    // {
    //     MSG_BOX("Failed to create Light Component");
    //     return E_FAIL;
    // }
    // m_umComponent[ID_DYNAMIC].insert({ L"Com_Light", m_pLightCom });

    return S_OK;
}

CLightObject* CLightObject::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLightObject* LightObj = new CLightObject(pGraphicDev);

    if (FAILED(LightObj->Ready_GameObject()))
    {
        MSG_BOX("CLightObject Create Failed");
        Safe_Release(LightObj);
    }

    return LightObj;
}

void CLightObject::Free()
{
    // for (auto& component : m_umComponent[ID_DYNAMIC])
    // {
    //     Safe_Release(component.second);
    // }
    // m_umComponent[ID_DYNAMIC].clear();

    Engine::CGameObject::Free();
}
