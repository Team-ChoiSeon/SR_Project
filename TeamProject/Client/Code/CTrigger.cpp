#pragma once
#include "pch.h"
#include "CTrigger.h"

CTrigger::CTrigger(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CTrigger::CTrigger(const CTrigger& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CTrigger::~CTrigger()
{
}


