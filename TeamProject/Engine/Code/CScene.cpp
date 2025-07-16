#include "CScene.h"

#include "CRenderMgr.h"
#include "CCollisionMgr.h"

CScene::CScene(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CScene::~CScene()
{
    Free();
}


void CScene::Init_Layers()
{
    for (auto i = 0; i < LAYER_END; i++)
    {
        if (m_umLayer.find((LAYERID)i) == m_umLayer.end()) {
            CLayer* pLayer = CLayer::Create();
            m_umLayer.insert({(LAYERID)i,pLayer});
        }
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

void CScene::Clear_Layers()
{
    for (auto& scene : m_umLayer)
    {
        Safe_Release(scene.second);
    }
    m_umLayer.clear();
}

HRESULT CScene::Ready_Scene()
{
    // 레이어 Create할때 Ready_Layer 하기때문에 Ready_Layer X
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

void CScene::Exit_Scene()
{
    CRenderMgr::Get_Instance()->Clear();
    CCollisionMgr::Get_Instance()->Clear();
}

HRESULT CScene::LoadScene(CScene* from, CScene* to)
{
    return E_FAIL;
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
