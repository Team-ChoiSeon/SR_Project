#pragma once
#include "Engine_Define.h"
#include "CLightMgr.h"

IMPLEMENT_SINGLETON(CLightMgr)

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
    //std::list<CLight*> sortedLights(m_LightList);
    //
    //sortedLights.remove_if([](CLight* light) {
    //    return light->Get_LightType() == CLight::DIRECTIONAL_LIGHT;
    //    });
    //
    // sortedLights.sort([&](CLight* a, CLight* b) {
    //     D3DXVECTOR3 posA = a->Get_LightInfo().Position;
    //     D3DXVECTOR3 posB = b->Get_LightInfo().Position;
    //
    //     D3DXVECTOR3 vecA = cameraPos - posA;
    //     D3DXVECTOR3 vecB = cameraPos - posB;
    //
    //     float distA = D3DXVec3Length(&vecA);
    //     float distB = D3DXVec3Length(&vecB);
    //
    //     return distA < distB;
    //     });

    int index = 0;
    for (auto light : m_LightList) {
        light->EnableLight(true);
        light->LateUpdate_Component();
    }
    // for (auto light : sortedLights) {
    //     if (index >= 7) 
    //         break;
    //     light->EnableLight(true);
    //     ++index;
    // }

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
