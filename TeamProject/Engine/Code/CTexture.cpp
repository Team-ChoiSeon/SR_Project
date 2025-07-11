#include "CTexture.h"

CTexture::CTexture()
{
}

CTexture::~CTexture()
{
}

CTexture* CTexture::Create()
{
    CTexture* pTex = new CTexture();
    if (FAILED(pTex->Ready_Texture()))
    {
        Safe_Release(pTex);
        return nullptr;
    }
    return pTex;
}

HRESULT CTexture::Ready_Texture()
{
    return S_OK;
}

HRESULT CTexture::Load(LPDIRECT3DDEVICE9 pDevice, const wstring& filePath)
{
    if (!pDevice) return E_FAIL;

    LPDIRECT3DTEXTURE9 pTexture = nullptr;

    std::wstring debugMessage = L"[Texture Load Failed] Path: " + filePath + L"\n";

    if (FAILED(D3DXCreateTextureFromFileW(pDevice, filePath.c_str(), &pTexture)))
    {
        OutputDebugStringW(debugMessage.c_str());
        MSG_BOX("CTexture::Load Failed : CreateTextureFromFile");
        return E_FAIL;
    }
    m_pTexture = pTexture;
    return S_OK;
}

void CTexture::Bind(LPDIRECT3DDEVICE9 pDevice, _uint stage)
{
    if (m_pTexture == nullptr)
    {
        MSG_BOX("CTexture::Bind Fail : m_pTexture == nullptr");
        return;
    }

    if (pDevice == nullptr)
    {
        MSG_BOX("CTexture::Bind Fail : pDevice == nullptr");
        return;
    }

    pDevice->SetTexture(stage, m_pTexture);
}

void CTexture::Free()
{
	CResource::Free();
}
