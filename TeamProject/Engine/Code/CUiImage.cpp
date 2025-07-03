#include "CUiImage.h"

CUiImage::CUiImage()
{
}

CUiImage::CUiImage(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev) 
{
}

CUiImage::CUiImage(const CUiImage& rhs) : CUI(rhs), m_pTexture(rhs.m_pTexture) 
{

	if (m_pSprite)
		m_pSprite->AddRef();
}

CUiImage::~CUiImage() 
{ 
	Free(); 
}

HRESULT CUiImage::Ready_Image(CTexture* pTex)
{

	// if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
	// 	return E_FAIL;
	
	if (FAILED(Ready_UI()))
	{
		OutputDebugString(_T("CUiImage::Ready_UI() failed\n"));
		return E_FAIL;
	}

	m_pTexture = pTex; 
	if (m_pTexture)
	{
		OutputDebugString(_T("m_pTexture is set successfully.\n"));
	}
	else
	{
		OutputDebugString(_T("m_pTexture is NULL!\n"));
	}

	return S_OK;
}

void CUiImage::Update_Component(const _float& fTimeDelta) 
{
}

void CUiImage::LateUpdate_Component() 
{
}

void CUiImage::Render_Component()
{
	if (!m_bVisible || !m_pSprite || !m_pTexture)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	LPDIRECT3DTEXTURE9 pTex = static_cast<LPDIRECT3DTEXTURE9>(m_pTexture->Get_Texture());

	if (pTex)
	{
		_matrix matScale, matTrans, matWorld;
		D3DXMatrixScaling(&matScale, m_vScale.x, m_vScale.y, 1.f);
		D3DXMatrixTranslation(&matTrans, m_vPosition.x, m_vPosition.y, 0.f);
		matWorld = matScale * matTrans;

		m_pSprite->SetTransform(&matWorld);
		D3DXVECTOR3 center(20.f, 20.f, 0.f);
		m_pSprite->Draw(pTex, nullptr, &center, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));
	}

	m_pSprite->End();
}

CUiImage* CUiImage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUiImage* pUiImage = new CUiImage(pGraphicDev);

	// if (FAILED(pUiImage->Ready_Image()))
	// {
	//     Safe_Release(pUiImage);
	//     MSG_BOX("UI Create Failed");
	//     return nullptr;
	// }

	return pUiImage;
}

void CUiImage::Free()
{
	m_pTexture = nullptr; 
	Safe_Release(m_pSprite);
}
