#include "CScene.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
}

HRESULT CScene::Ready_Scene()
{

    return S_OK;
}

void CScene::Create_Layer()
{
    for (auto i = 0; i < LAYER_END; i++)
    {
        CLayer* pLayer = CLayer::Create();
        m_umLayer.insert({(LAYERID)i,pLayer});
    }
}

void CScene::Add_Layer(LAYERID eID)
{
    if (m_umLayer.find(eID) != m_umLayer.end())
    {
        MSG_BOX("[CScene] LAYERID ");
        return;
    }
    CLayer* pLayer = CLayer::Create();
    m_umLayer.insert({ eID, pLayer });
}

void CScene::Remove_Layer(LAYERID eID)
{
    auto iter = m_umLayer.find(eID);
    if (iter == m_umLayer.end())
        return;

    Safe_Release(iter->second);
    m_umLayer.erase(iter);
}

_int CScene::Update_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_umLayer)
        pLayer.second->Update_Layer(fTimeDelta);

    return 0;
}

void CScene::LateUpdate_Scene(const _float& fTimeDelta)
{
    for (auto& pLayer : m_umLayer)
        pLayer.second->LateUpdate_Layer(fTimeDelta);
}

void CScene::Render_Scene()
{
    for (const auto& pLayer : m_umLayer)
        pLayer.second->Render_Layer();
}

void CScene::Free()
{
    for (auto& scene : m_umLayer)
    {
        Safe_Release(scene.second);
    }
    m_umLayer.clear();
    Safe_Release(m_pGraphicDev);
}
