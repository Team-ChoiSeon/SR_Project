#pragma once
#include "CResource.h"
class CTexture : public CResource
{
protected:
    explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);

public:
    virtual ~CTexture();

public:
    CResource* Get_TextureResource(_uint iIndex = 0) const
    {
        if (iIndex >= m_vecTexture.size())
            return nullptr;

        return const_cast<CTexture*>(this); // this를 CResource*로 반환
    }

    LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex = 0) const
    {
        if (iIndex >= m_vecTexture.size())
            return nullptr;

        return m_vecTexture[iIndex];
    }

public:
    static CTexture* Create(LPDIRECT3DDEVICE9 pGraphicDev, const wstring& filePath, TEXTUREID eType, _uint iCnt);
    
    HRESULT Ready_Texture(const wstring& filePath, TEXTUREID eType, _uint iCnt);
    
    virtual void Free() override;


private:
    vector<LPDIRECT3DBASETEXTURE9> m_vecTexture;
};

