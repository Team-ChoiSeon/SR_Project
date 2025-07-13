#pragma once
#include "pch.h"
#include "CSensor.h"

CSensor::CSensor(LPDIRECT3DDEVICE9 pGraphicDev)
	: CGameObject(pGraphicDev)
{
}

CSensor::CSensor(const CSensor& rhs)
	: CGameObject(rhs.m_pGraphicDev)
{
}

CSensor::~CSensor()
{
}
