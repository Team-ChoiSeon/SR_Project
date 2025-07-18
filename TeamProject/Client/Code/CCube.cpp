#include "pch.h"
#include "CCube.h"
#include "CCollider.h"
#include "CRigidBody.h"

CCube::CCube(LPDIRECT3DDEVICE9 pGraphicDev)
    : CGameObject(pGraphicDev), m_pModel(nullptr), m_pTransform(nullptr)
{
}

CCube::~CCube()
{
}

HRESULT CCube::Ready_GameObject()
{
    return E_NOTIMPL;
}

_int CCube::Update_GameObject(const _float& fTimeDelta)
{
    return _int();
}

void CCube::LateUpdate_GameObject(const _float& fTimeDelta)
{
}

void CCube::Set_Tap(bool Trigger)
{
	m_bTap = Trigger;
}

void CCube::Set_Away(bool Trigger)
{
    m_bAway = Trigger;
}

