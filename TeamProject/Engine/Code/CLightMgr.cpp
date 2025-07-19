#pragma once
#include "Engine_Define.h"
#include "CLightMgr.h"
#include "CCameraMgr.h"
#include "CGameObject.h"
#include "CTransform.h"

IMPLEMENT_SINGLETON(CLightMgr)

CLightMgr::CLightMgr()
{
}

CLightMgr::~CLightMgr()
{
    Free();
}

HRESULT CLightMgr::Ready_Light(LPDIRECT3DDEVICE9 pDevice)
{
    m_pDevice = pDevice;

    // [기본 방향 라이트 설정]

    ZeroMemory(&m_BaseLight, sizeof(D3DLIGHT9));
    m_BaseLight.Type = D3DLIGHT_DIRECTIONAL;

    // 빛의 방향 (정규화된 벡터)
    _vec3 dir = D3DXVECTOR3(1.f, 2.f, 1.f); 
    D3DXVec3Normalize(&dir, &dir);
    m_BaseLight.Direction = dir;
    m_BaseLight.Diffuse = D3DXCOLOR(1.f, 1.f, 1.f, 1.f);  // 흰색 디퓨즈
    m_BaseLight.Specular = D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.f);
    m_BaseLight.Ambient = D3DXCOLOR(0.6f, 0.6f, 0.6f, 1.f); // 약한 앰비언트

    // 0번 라이트로 적용
    m_pDevice->SetLight(0, &m_BaseLight);
    m_pDevice->LightEnable(0, TRUE);
    fAngle = 30;
    return S_OK;
}

void CLightMgr::AddLight(CLight* pLight)
{
    m_LightList.push_back(pLight);
}

void CLightMgr::UpdateLights(const _float& fTimeDelta)
{
   //if (CGameObject* pCam = CCameraMgr::Get_Instance()->Get_MainCamera()) {
   //   _vec3 vCamLook=  pCam->Get_Component<CTransform>()->Get_Info(INFO_LOOK);
   //   vCamLook *=cosf(D3DXToRadian(fAngle));
   //   m_BaseLight.Direction = vCamLook;
   //}
   //else {
   //    m_BaseLight.Direction = { 0.f,1.f,0.f };
   //}
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

    //int index = 0;
    //for (auto light : m_LightList) {
    //    light->EnableLight(true);
    //    light->LateUpdate_Component(0.f);
    //}
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
