#pragma once
#include "pch.h"
#include "CWeightButton.h"
#include "CTransform.h"
#include "CCollider.h"
#include "CRigidBody.h"

CWeightButton::CWeightButton(LPDIRECT3DDEVICE9 pGraphicDev)
	: CActiveTrigger(pGraphicDev)
{
}

CWeightButton::CWeightButton(const CWeightButton& rhs)
    : CActiveTrigger(rhs.m_pGraphicDev)
{
}

CWeightButton::~CWeightButton()
{
}

HRESULT CWeightButton::Ready_GameObject()
{
    Add_Component<CTransform>(ID_DYNAMIC, m_pGraphicDev);
	Add_Component<CModel>(ID_DYNAMIC, m_pGraphicDev);
    Add_Component<CRigidBody>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CTransform>());
    Add_Component<CCollider>(ID_DYNAMIC, m_pGraphicDev, Get_Component<CRigidBody>());



	m_pTransform = Get_Component<CTransform>();
	m_pModel = Get_Component<CModel>();
	m_pCollider = Get_Component<CCollider>();
    m_pRigid = Get_Component<CRigidBody>();

    m_pTransform->Set_Pos({ -5.f, 20.f, 20.f });
    m_pTransform->Set_Angle({ 0.f, 0.f, 0.f });
	m_pTransform->Set_Scale({ 1.f, 0.3f, 1.f });

    m_fTimeLimit = 0.f;
    m_fCount = 1.f;
    m_fWeight = 0.f;
    m_fRecycleCount = -1.f;
    m_bTriggerOn = false;

    return S_OK;
}

_int CWeightButton::Update_GameObject(const _float& fTimeDelta)
{
    ComputeConditions();
    return S_OK;
}

void CWeightButton::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

CWeightButton* CWeightButton::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
    CWeightButton* pButton = new CWeightButton(pGraphicDev);
    if (FAILED(pButton->Ready_GameObject()))
    {
        Safe_Release(pButton);
        MSG_BOX("WButton Create Failed");
        return nullptr;
    }
    return pButton;
}

void CWeightButton::Free()
{
    Safe_Release(m_pTransform);
	Safe_Release(m_pModel);
	Safe_Release(m_pCollider);
    Safe_Release(m_pRigid);
    Safe_Release(m_pGraphicDev);
}

void CWeightButton::Set_Info(const _vec3& startpos, const _float& weight, const _float& recyclecount)
{
    m_pTransform->Set_Pos(startpos);
    m_fWeight = weight;
    m_fRecycleCount = recyclecount;
}

void CWeightButton::Set_Trigger(const _float& weight)
{
    if (m_fRecycleCount < 0.f) {
        if (m_fWeight <= weight)
        {
            m_pTransform->Set_Scale({ 1.f, 0.1f, 1.f });
            m_pTransform->Set_Pos(m_pTransform->Get_Pos() + _vec3(0.f, -0.1f, 0.f));
            m_bWeight = true;
        }
        else
        {
            m_bWeight = false;
        }
    }
    else if (m_fRecycleCount == 0.f)
        m_bRecycleCount = false;
    else if (m_fRecycleCount > 0.f)
    {
        if (m_fWeight <= weight)
        {
            m_pTransform->Set_Scale({ 1.f, 0.1f, 1.f });
            m_pTransform->Set_Pos(m_pTransform->Get_Pos() + _vec3(0.f, -0.1f, 0.f));
            m_bWeight = true;
			m_fRecycleCount --;
        }
        else
        {
            m_bWeight = false;
        }
    }

    m_bTimeLimit = true;
    m_bCount = true;
    m_bSequence = true;
}

void CWeightButton::Checkcollision()
{
	
}
