#include "pch.h"
#include "CPickSwitch.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPickTarget.h"
#include "CFactory.h"
CPickSwitch::CPickSwitch(LPDIRECT3DDEVICE9 pGraphicDev)
	: CSwitch(pGraphicDev)
{
}

CPickSwitch::CPickSwitch(const CPickSwitch& rhs)
	: CSwitch(rhs.m_pGraphicDev)
{
}

CPickSwitch::~CPickSwitch()
{
}

HRESULT CPickSwitch::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();
    m_pTransform->Ready_Component();
    m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
    m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
    m_pTransform->Set_Scale({ 1.f, 1.f, 1.f });

    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();
    m_pRigid->Set_Friction(0.f);
    m_pRigid->Set_Mass(10.f);
    m_pRigid->Set_Bounce(0.f);
    m_pRigid->Set_OnGround(true);
    m_pRigid->Set_UseGravity(false);

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCollider = Get_Component<CCollider>();
    m_pCollider->Set_ColTag(ColliderTag::GROUND);
    m_pCollider->Set_ColType(ColliderType::PASSIVE);

    Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
    m_pPick = Get_Component<CPickTarget>();


    m_bGrab = false;                        //picking 확인
    m_bReusable = true;                     //여러번 상호작용 가능한지
    m_bButtonOn = false;                   //켜져있는지
    m_bPressed = false;
    m_bInit = false;



    CFactory::Save_Prefab(this, "CPickSwitch");
	return S_OK;
}

_int CPickSwitch::Update_GameObject(const _float& fTimeDelta)
{
    CGameObject::Update_GameObject(fTimeDelta);
    Init();
    SwitchOn(m_bGrab);
    Pressing(fTimeDelta);
	return _int();
}

void CPickSwitch::LateUpdate_GameObject(const _float& fTimeDelta)
{
    CGameObject::LateUpdate_GameObject(fTimeDelta);
}

CPickSwitch* CPickSwitch::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CPickSwitch* tButton = new CPickSwitch(pGraphicDev);
    if (FAILED(tButton->Ready_GameObject()))
    {
        Safe_Release(tButton);
        MSG_BOX("PickButton Create Failed");
        return nullptr;
    }
    return tButton;
}

void CPickSwitch::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pModel);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCollider);
    Safe_Release(m_pPick);
    Safe_Release(m_pGraphicDev);
}

void CPickSwitch::Init()
{
    //Set Default variables
    if (!m_bInit)
    {
        m_vOriginalPos = m_pTransform->Get_Pos();
        m_vOriginalScale = m_pTransform->Get_Scale();
        m_bInit = true;
    }
}


REGISTER_GAMEOBJECT(CPickSwitch)