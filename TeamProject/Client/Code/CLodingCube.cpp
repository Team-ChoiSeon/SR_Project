#include "pch.h"
#include "CLodingCube.h"
#include "CFactory.h"

CLodingCube::CLodingCube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev)
{
}

CLodingCube::~CLodingCube()
{
    Free();
}

HRESULT CLodingCube::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();

    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCollider = Get_Component<CCollider>();
    m_pCollider->Set_ColTag(ColliderTag::NONE);
    m_pCollider->Set_ColType(ColliderType::PASSIVE);

    m_pTransform->Set_Scale({ 0.1f, 0.1f, 0.1f });
    //m_pTransform->Set_Scale({ 0.08f, 0.08f, 0.08f });
    m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });

    m_pRigid->Set_OnGround(true);
    m_pRigid->Set_UseGravity(false);

    m_vAxis = _vec3(
        (rand() % 200 - 100) / 100.f, // -1.0f ~ 1.0f
        (rand() % 200 - 100) / 100.f,
        (rand() % 200 - 100) / 100.f
    );

    D3DXVec3Normalize(&m_vAxis, &m_vAxis);


    //CFactory::Save_Prefab(this, "CLodingCube");

    return S_OK;
}

int CLodingCube::Update_GameObject(const _float& fTimeDelta)
{

    m_pTransform->Rotate_Axis(m_vAxis, D3DXToRadian(100.f) * fTimeDelta);
    //m_pTransform->Rotate_Axis({ 0.f, 1.f, 0.f }, D3DXToRadian(360.f) * fTimeDelta);


    CGameObject::Update_GameObject(fTimeDelta);
    return 0;
}

void CLodingCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CLodingCube* CLodingCube::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CLodingCube* pCube = new CLodingCube(pGraphicDev);

    if (FAILED(pCube->Ready_GameObject()))
    {
        Safe_Release(pCube);
        MSG_BOX("CLodingCube Create Failed");
        return nullptr;
    }

    return pCube;
}

void CLodingCube::Free()
{
    Safe_Release(m_pRigid);
    Safe_Release(m_pModel);
    Safe_Release(m_pCollider);
    Safe_Release(m_pTransform);

    CGameObject::Free();
}

//REGISTER_GAMEOBJECT(CLodingCube)
