#pragma once
#include "CGameObject.h"

namespace Engine {
    class CTransform;
    class CModel;
    class CCollider;
    class CRigidBody;
}

class CBlock :
    public Engine::CGameObject
{
protected:
    explicit CBlock(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CBlock();
public:

protected:
    CTransform* m_pTransform;
    CModel* m_pModel;
    CCollider* m_pCollider;
    CRigidBody* m_pRigid;
};

