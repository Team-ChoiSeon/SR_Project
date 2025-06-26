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
