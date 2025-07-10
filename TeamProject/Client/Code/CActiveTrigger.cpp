#pragma once
#include "pch.h"
#include "CActiveTrigger.h"

CActiveTrigger::CActiveTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: CCube(pGraphicDev)
{
}

CActiveTrigger::CActiveTrigger(const CActiveTrigger& rhs)
	: CCube(rhs.m_pGraphicDev)
{
}

CActiveTrigger::~CActiveTrigger()
{
}

void CActiveTrigger::ComputeConditions()
{
	if (m_bTimeLimit && m_bCount && m_bWeight && m_bRecycleCount && m_bSequence)
		m_bTriggerOn = true;
	else
		m_bTriggerOn = false;
}
