#include "CTexture.h"

CTexture::CTexture(LPDIRECT3DDEVICE9 pGraphicDev)
	: CResource(pGraphicDev)
{
}

CTexture::~CTexture()
{
}

CTexture* CTexture::Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& filePath, TEXTUREID eType, _uint iCnt)
{
    CTexture* pTex = new CTexture(pGraphicDev);
    if (FAILED(pTex->Ready_Texture(filePath, eType, iCnt)))
    {
        Safe_Release(pTex);
        return nullptr;
    }
    return pTex;
}

HRESULT CTexture::Ready_Texture(const wstring& filePath, TEXTUREID eType, _uint iCnt)
{
    m_vecTexture.reserve(iCnt);

    for (_uint i = 0; i < iCnt; ++i)
    {
        LPDIRECT3DBASETEXTURE9 pTexture = nullptr;

        if (eType == TEX_NORMAL)
        {
            if (FAILED(D3DXCreateTextureFromFileW(m_pGraphicDev, filePath.c_str(), (LPDIRECT3DTEXTURE9*)&pTexture)))
                return E_FAIL;
        }
        else if (eType == TEX_CUBE)
        {
            if (FAILED(D3DXCreateCubeTextureFromFileW(m_pGraphicDev, filePath.c_str(), (LPDIRECT3DCUBETEXTURE9*)&pTexture)))
                return E_FAIL;
        }

        m_vecTexture.push_back(pTexture);

    }

    return S_OK;
}

void CTexture::Free()
{
	CResource::Free();
}
