#pragma once
#include "pch.h"
#include "CTimerButton.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"
#include "CPickTarget.h"

CTimerButton::CTimerButton(LPDIRECT3DDEVICE9 pGraphicDev)
    : CActiveTrigger(pGraphicDev)
{
}

CTimerButton::CTimerButton(const CTimerButton& rhs)
    : CActiveTrigger(rhs.m_pGraphicDev)
{
}

CTimerButton::~CTimerButton()
{
}

HRESULT CTimerButton::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CTransform>());
    Add_Component<CPickTarget>(ID_DYNAMIC, m_pGraphicDev, RAY_AABB);
    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CRigidBody>());


    m_pTransform = Get_Component<CTransform>();
    m_pModel = Get_Component<CModel>();
    m_pCollider = Get_Component<CCollider>();
    m_pRigid = Get_Component<CRigidBody>();
    m_pPick = Get_Component<CPickTarget>();

    m_pTransform->Set_Pos({ 0.f, 0.f, 0.f });
    m_pTransform->Set_Look({ 0.f, 0.f, 1.f });
    m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
    m_pTransform->Set_Scale({ 1.f, 0.3f, 1.f });

    m_fTimeLimit = 0.f;
    m_fCount = 1.f;
    m_fWeight = 0.f;
    m_fRecycleCount = -1.f;
    m_bTriggerOn = false;
    m_bTimeLimit = false;

    return S_OK;

}

_int CTimerButton::Update_GameObject(const _float& fTimeDelta)
{
    ComputeConditions();
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->Update_Component(fTimeDelta);
    return S_OK;
}

void CTimerButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
    for (auto& pComponent : m_umComponent[ID_DYNAMIC])
        pComponent.second->LateUpdate_Component();
}

CTimerButton* CTimerButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CTimerButton* pButton = new CTimerButton(pGraphicDev);
    if (FAILED(pButton->Ready_GameObject()))
    {
        Safe_Release(pButton);
        MSG_BOX("TButton Create Failed");
        return nullptr;
    }
    return pButton;
}

void CTimerButton::Free()
{
    Safe_Release(m_pTransform);
    Safe_Release(m_pModel);
    Safe_Release(m_pCollider);
    Safe_Release(m_pRigid);
    Safe_Release(m_pPick);
    Safe_Release(m_pGraphicDev);
}

void CTimerButton::Set_Info(const _vec3& startpos, const _float& timelimit, const _float& recyclecount)
{
    m_pTransform->Set_Pos(startpos);
    m_fTimeLimit = timelimit;
    m_fRecycleCount = recyclecount;
}

void CTimerButton::Set_Trigger(_float& time)
{
    if (m_fRecycleCount == 0.f)
        m_bRecycleCount = false;
    else {
        if (time <= m_fTimeLimit) {
            m_pTransform->Set_Scale({ 1.f, 0.1f, 1.f });
            m_bTimeLimit = true;
        }
        else
        {
            m_pTransform->Set_Scale({ 1.f, 0.3f, 1.f });
            m_bTimeLimit = false;
            time = 0.f;
            
            if (m_fRecycleCount > 0)
                m_fRecycleCount--;
        }
        m_bRecycleCount = true;
    }

    m_bWeight = true;
    m_bCount = true;
    m_bSequence = true;
}

void CTimerButton::CheckActive()
{
}
