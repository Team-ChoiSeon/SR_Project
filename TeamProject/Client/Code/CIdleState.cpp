#pragma once
#include "pch.h"
#include "CIdleState.h"

#include "CVellum.h"

#include "CChaseState.h"
#include "CDiveState.h"
#include "CSpinState.h"
#include "CFireState.h"

void CIdleState::Enter(CVellum* pVellum)
{
	OutputDebugString(L"Idle : Enter\n");
	m_fPatternTime = 0.f;
}

void CIdleState::Update(const _float fTimeDelta, CVellum* pVellum)
{
	m_fPatternTime += fTimeDelta;
	if (m_fPatternTime >= m_fSwitchTime)
	{
		if(pVellum->Get_PatternIdx() > 3)
		{
			// 패턴 변경
			m_ePattern = static_cast<VPattern>(rand() % 4);
		}
		else
		{
			m_ePattern = static_cast<VPattern>(pVellum->Get_PatternIdx());
			pVellum->Set_PatternIdx(pVellum->Get_PatternIdx() + 1);
		}

		switch (m_ePattern)
		{
		case VPattern::CHASE:
			m_pNext = new CChaseState();
			break;
		case VPattern::DIVE:
			m_pNext = new CDiveState();
			break;
		case VPattern::SPIN:
			m_pNext = new CSpinState();
			break;
		case VPattern::FIRE:
			m_pNext = new CFireState();
			break;
		}

		pVellum->Change_Pattern(m_pNext);
		return;
	}
}

void CIdleState::Exit(CVellum* pVellum)
{
	m_pNext = nullptr;
	OutputDebugString(L"Idle : Exit\n");
}
