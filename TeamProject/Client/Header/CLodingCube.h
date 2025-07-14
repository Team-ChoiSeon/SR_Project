#pragma once
#include "CGameObject.h"

#include "CTransform.h"
#include "CModel.h"
#include "CCollider.h"
#include "CRigidBody.h"

class CLodingCube : public Engine::CGameObject
{
private:
    CLodingCube(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CLodingCube();

public:
    HRESULT Ready_GameObject() override;
    int Update_GameObject(const _float& fTimeDelta) override;
    void LateUpdate_GameObject(const _float& fTimeDelta) override;

    static CLodingCube* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    void Free();

private:
    CTransform* m_pTransform;
    CModel* m_pModel;
    CCollider* m_pCollider;
    CRigidBody* m_pRigid;

    _vec3 m_vAxis;
};
