#pragma once
#include "CLightMgr.h"


CLightMgr::CLightMgr()
{
}

CLightMgr::~CLightMgr()
{
    Free();
}

void CLightMgr::AddLight(CLight* pLight)
{
    m_LightList.push_back(pLight);
}

void CLightMgr::UpdateLights(const D3DXVECTOR3& cameraPos)
{
    m_LightList.remove_if([](CLight* light) {
        return light->Get_LightType() == CLight::DIRECTIONAL_LIGHT;
        });

    // m_LightList.sort([&](CLight* a, CLight* b) {
    //     float distA = D3DXVec3Length(&(cameraPos - a->Get_LightInfo().Position));
    //     float distB = D3DXVec3Length(&(cameraPos - b->Get_LightInfo().Position));
    //     return distA < distB;
    //     });

    int index = 0;
    for (auto light : m_LightList) {
        light->EnableLight(index < 7);
        ++index;
    }
}

void CLightMgr::Free()
{
    for (auto& light : m_LightList) {
        Safe_Release(light);
    }
    m_LightList.clear();
    // for_each(m_LightList.begin(), m_LightList.end(), CDeleteObj());
    // m_LightList.clear();
}
