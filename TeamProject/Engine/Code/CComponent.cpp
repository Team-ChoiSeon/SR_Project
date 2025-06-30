#include "CComponent.h"

CComponent::CComponent()
    : m_pGraphicDev(nullptr)
{
}

CComponent::CComponent(LPDIRECT3DDEVICE9 pGraphicDev)
    : m_pGraphicDev(pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CComponent::CComponent(const CComponent& rhs)
    : m_pGraphicDev(rhs.m_pGraphicDev)
{
    m_pGraphicDev->AddRef();
}

CComponent::~CComponent()
{
}
