#include "CUi.h"

CUI::CUI() : m_pSprite(nullptr), m_vPosition(0.f, 0.f), m_vScale(1.f, 1.f), m_bVisible(true)
{
	D3DXMatrixIdentity(&m_matOrtho);
}

CUI::CUI(LPDIRECT3DDEVICE9 pGraphicDev) : CComponent(pGraphicDev), m_pSprite(nullptr), m_vPosition(0.f, 0.f), m_vScale(1.f, 1.f), m_bVisible(true)
{
	D3DXMatrixIdentity(&m_matOrtho);
}

CUI::CUI(const CUI& rhs) : CComponent(rhs), m_pSprite(rhs.m_pSprite), m_vPosition(rhs.m_vPosition), m_vScale(rhs.m_vScale), m_bVisible(rhs.m_bVisible), m_matOrtho(rhs.m_matOrtho)
{
	if (m_pSprite)
		m_pSprite->AddRef();
}

CUI::~CUI()
{
	Free();
}

HRESULT CUI::Ready_Component()
{
	// if (!m_pGraphicDev)
	// {
	// 	OutputDebugString(_T("m_pGraphicDev is NULL in Ready_UI()\n"));
	// 	return E_FAIL;
	// }


	//	OutputDebugString(_T("D3DXCreateSprite succeeded in Ready_UI()\n"));

	return S_OK;
}



void CUI::Render(LPDIRECT3DDEVICE9 pDevice)
{
	if (!m_bVisible || !m_pSprite)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	m_pSprite->End();
}


void CUI::Free()
{
	Safe_Release(m_pSprite);
}