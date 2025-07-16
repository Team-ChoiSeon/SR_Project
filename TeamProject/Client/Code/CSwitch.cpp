#pragma once
#include "pch.h"
#include "CSwitch.h"
#include "CTransform.h"

CSwitch::CSwitch(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
    //CSoundMgr::Get_Instance()->Load_Sound("active", "../Bin/Resource/Sound/Active3.mp3");
}

CSwitch::CSwitch(const CSwitch& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CSwitch::~CSwitch()
{
}

void CSwitch::SwitchOn(bool trigger)
{
    if (m_bReusable)
    {
        if (trigger)
        {
            if (!m_bPressed)
            {
                m_bPressed = true;
                m_bButtonOn = !m_bButtonOn;
               //CSoundMgr::Get_Instance()->Play("active", "EFFECT", false);
            }
        }
        else
        {
            m_bPressed = false;
        }
    }
    else
    {
        m_bButtonOn = true;
    }
}

void CSwitch::Pressing(const _float& fTimeDelta)
{
    // Pressing Motion
    _float pressScale = m_vOriginalScale.y * 0.5f;
    _float curScale = m_pTransform->Get_Scale().y;

    if (m_bButtonOn && curScale > pressScale) {
        curScale = max(curScale - fTimeDelta, pressScale);
    }
    else if (!m_bButtonOn && curScale < m_vOriginalScale.y) {
        curScale = min(curScale + fTimeDelta, m_vOriginalScale.y);
    }

    m_pTransform->Set_ScaleY(curScale);
    m_pTransform->Set_PosY(m_vOriginalPos.y - (m_vOriginalScale.y - curScale));
}
