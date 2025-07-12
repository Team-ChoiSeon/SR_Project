#include "CUiImage.h"
#include "CRenderMgr.h"

CUiImage::CUiImage()
{
}

CUiImage::CUiImage(LPDIRECT3DDEVICE9 pGraphicDev) : CUI(pGraphicDev) 
{
}

CUiImage::CUiImage(const CUiImage& rhs) : CUI(rhs) 
{
	if (m_pSprite)
		m_pSprite->AddRef();
}

CUiImage::~CUiImage() 
{ 
	Free(); 
}

HRESULT CUiImage::Ready_Image()
{
	if (FAILED(D3DXCreateSprite(m_pGraphicDev, &m_pSprite)))
		return E_FAIL;

	return S_OK;
}

void CUiImage::Update_Component(const _float& fTimeDelta) 
{
}

void CUiImage::LateUpdate_Component(const _float& fTimeDelta)
{
	CRenderMgr::Get_Instance()->Add_UI(this);
}

void CUiImage::Render(LPDIRECT3DDEVICE9 pDevice)
{

	if (!m_bVisible || !m_pSprite || !m_Texture)
		return;

	m_pSprite->Begin(D3DXSPRITE_ALPHABLEND);

	LPDIRECT3DTEXTURE9 pTex = m_Texture;

		D3DSURFACE_DESC desc = {};
		pTex->GetLevelDesc(0, &desc);

		float width = static_cast<float>(desc.Width);
		float height = static_cast<float>(desc.Height);

		D3DXVECTOR2 scale = m_vScale;
		D3DXVECTOR2 center(width * 0.5f, height * 0.5f);
		D3DXVECTOR2 pos = m_vPosition;

		D3DXMATRIX mat;
		D3DXMatrixTransformation2D(&mat, &center, 0, &scale, nullptr, 0, &pos);

		m_pSprite->SetTransform(&mat);
		m_pSprite->Draw(pTex, nullptr, nullptr, nullptr, D3DCOLOR_ARGB(255, 255, 255, 255));

		D3DXMATRIX identity;
		D3DXMatrixIdentity(&identity);
		m_pSprite->SetTransform(&identity);
	m_pSprite->End();

}

void CUiImage::Set_Texture(CTexture* pTex) 
{

	m_Texture = static_cast<LPDIRECT3DTEXTURE9>(pTex->Get_Texture());

}

LPDIRECT3DTEXTURE9 CUiImage::Get_Texture() const
{
	return m_Texture;
}

CUiImage* CUiImage::Create(LPDIRECT3DDEVICE9 pGraphicDev)
{
	CUiImage* pUiImage = new CUiImage(pGraphicDev);

	if (FAILED(pUiImage->Ready_Image()))
	{
	    Safe_Release(pUiImage);
	    MSG_BOX("UI Create Failed");
	    return nullptr;
	}

	return pUiImage;
}

void CUiImage::Free()
{
	Safe_Release(m_pSprite);
	
}
