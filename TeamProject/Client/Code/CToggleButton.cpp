#include "pch.h"
#include "CToggleButton.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPickTarget.h"

CToggleButton::CToggleButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CActiveTrigger(pGraphicDev)
{
}

CToggleButton::CToggleButton(const CToggleButton& rhs)
	: CActiveTrigger(rhs.m_pGraphicDev)
{
}

CToggleButton::~CToggleButton()
{
}

HRESULT CToggleButton::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    m_pTransform = Get_Component<CTransform>();
    m_pTransform->Ready_Component();
    m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
    m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
    m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
    m_pTransform->Set_Scale({ 1.f, 0.3f, 1.f });

    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    m_pModel = Get_Component<CModel>();

    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, m_pTransform);
    m_pRigid = Get_Component<CRigidBody>();

    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, m_pRigid);
    m_pCollider = Get_Component<CCollider>();

    Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
    m_pPick = Get_Component<CPickTarget>();


    m_bGrab = false;                        //picking 확인
    m_bReusable = true;                     //여러번 상호작용 가능한지
    m_bTriggerOn = false;                   //켜져있는지

	return S_OK;
}

_int CToggleButton::Update_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);
    if (m_bReusable)
    {
        if (m_bGrab)
                m_bTriggerOn = !m_bTriggerOn;
    }
    else
    {
        m_bTriggerOn = true;
    }
	return _int();
}

void CToggleButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component();
}

CToggleButton* CToggleButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CToggleButton* tButton = new CToggleButton(pGraphicDev);
    if (FAILED(tButton->Ready_GameObject()))
    {
        Safe_Release(tButton);
        MSG_BOX("TGButton Create Failed");
        return nullptr;
    }
    return tButton;
}

void CToggleButton::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pModel);
    Safe_Release(m_pRigid);
    Safe_Release(m_pCollider);
    Safe_Release(m_pPick);
    Safe_Release(m_pGraphicDev);
}
