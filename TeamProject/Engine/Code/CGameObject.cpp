#include "CGameObject.h"

CGameObject::CGameObject(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CGameObject::CGameObject(const CGameObject& rhs)
    : m_pGraphicDev(rhs.m_pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CGameObject::~CGameObject()
{
}

template<typename T>
T* CGameObject::Get_Component(const wstring& pComponentTag)
{
    for (_uint i = 0; i < ID_END; ++i)
    {
        auto iter = m_umComponent[i].find(pComponentTag);
        if (iter != m_umComponent[i].end())
            return dynamic_cast<T*>(iter->second);
    }

    MSG_BOX("[CGameObject] Get_Component 실패 : nullptr 전달됨");
    return nullptr;
}

template<typename T, typename... Args>
void CGameObject::Add_Component(const wstring& pComponentTag, COMPONENTID eID, Args&&... args)
{
    std::unique_ptr<T> pComp = std::make_unique<T>(std::forward<Args>(args)...);
    if (pComp == nullptr)
    {
        MSG_BOX("[GameObject] Add_Component 실패 : 생성 실패");
        return;
    }


    if (m_umComponent[eID].find(pComponentTag) != m_umComponent[eID].end())
    {
        MSG_BOX("[GameObject] Add_Component 실패: 중복 태그");
        return;
    }

    m_umComponent[eID].emplace(pComponentTag, pComp.release());
}

HRESULT CGameObject::Ready_GameObject()
{
    return S_OK;
}

_int CGameObject::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);

    return 0;
}

void CGameObject::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component();
}

void CGameObject::Free()
{
    for (_uint i = 0; i < ID_END; ++i)
    {
        for (auto& [tag, comp] : m_umComponent[i])
            Safe_Release(comp);
    }

    Safe_Release(m_pGraphicDev);
}
