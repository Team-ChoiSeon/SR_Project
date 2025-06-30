#pragma once
#include "CGameObject.h"
#include "Engine_Define.h"

namespace Engine
{
    class CTransform;
    class CLight;
}

class CLightObject : public Engine::CGameObject
{
private:
    explicit CLightObject(LPDIRECT3DDEVICE9 pGraphicDev);
    explicit CLightObject(const CGameObject& rhs);
    virtual ~CLightObject();

public:
    virtual HRESULT Ready_GameObject();
    virtual _int Update_GameObject(const _float& fTimeDelta);
    virtual void LateUpdate_GameObject(const _float& fTimeDelta);
    virtual void Render_GameObject();

    void Set_Light(const D3DLIGHT9& tLightData);
    CLight* Get_LightComponent();

private:
    HRESULT Add_Component();

public:
    static CLightObject* Create(LPDIRECT3DDEVICE9 pGraphicDev);

private:
    Engine::CTransform* m_pTransformCom;
    Engine::CLight* m_pLightCom;

private:
    virtual void Free();
};
