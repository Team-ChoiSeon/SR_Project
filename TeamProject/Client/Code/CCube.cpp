#include "pch.h"
#include "CCube.h"

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

void CCube::Check_Lay()
{
    if (m_bCurGrab)
    {
        if (!m_bLay)
        {
            m_bLay = true;
        }
    }
    else
    {
        m_bLay = false;
    }
}
