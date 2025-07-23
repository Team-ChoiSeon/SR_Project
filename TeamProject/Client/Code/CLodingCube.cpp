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
    m_pModel->Set_Model(L"Cube_A.obj", L"Cube_A.mtl");
    m_pModel->Get_Material()->Set_Shader(L"g_UVScale.fx");
    m_pModel->Set_UVScale({ 1,1,1,1 });
    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();
    m_pParticle = Add_Component<CParticle>(ID_DYNAMIC, m_pGraphicDev);

    m_pParticle->PreSet_Fire(400, .3f, .8f);
    //m_pParticle->PreSet_Spread(200, 0.4f, 1.f, 5.f);
    m_pParticle->Set_Size(0.05f);
    m_pParticle->Set_Speed(0.2f);

    m_pTransform->Set_Scale({ 0.1f, 0.1f, 0.1f });
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
    m_fElapsedTime += fTimeDelta;
    m_fScaleTime += fTimeDelta;

    m_pTransform->Rotate_Axis(m_vAxis, D3DXToRadian(100.f) * fTimeDelta);
    //m_pTransform->Rotate_Axis({ 0.f, 1.f, 0.f }, D3DXToRadian(360.f) * fTimeDelta);

    if (m_fElapsedTime > 0.5f) {
        m_pParticle->Set_Texture(L"UI/LED_Blue_on.png");
    }
     if (m_fElapsedTime > 1.5f) {
        m_pParticle->Set_Texture(L"UI/LED_Red_on.png");
    }
      if (m_fElapsedTime > 2.5f) {
        m_pParticle->Set_Texture(L"UI/LED_off.png");
    }
     if (m_fElapsedTime > 3.5f ) {
        m_pParticle->Set_Texture(L"UI/LED_on.png");
        m_fElapsedTime = 0;
    }

      _vec3 nowScale = m_pTransform->Get_Scale();
      _float speed = fTimeDelta * 0.05f;

      if (m_fScaleTime < 1.5f) {
          m_pTransform->Set_Scale({
              nowScale.x + speed,
              nowScale.y + speed,
              nowScale.z + speed
              });
      }
      else if (m_fScaleTime < 3.f) {
          m_pTransform->Set_Scale({
              nowScale.x - speed,
              nowScale.y - speed,
              nowScale.z - speed
              });
      }
      else {
          m_fScaleTime = 0.f; // 주기 반복
      }

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
    Safe_Release(m_pTransform);

    CGameObject::Free();
}

//REGISTER_GAMEOBJECT(CLodingCube)
