#pragma once
#include "CResource.h"
class CTexture : public CResource
{
protected:
    explicit CTexture(LPDIRECT3DDEVICE9 pGraphicDev);
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
    virtual void Free() override;


private:
    vector<LPDIRECT3DBASETEXTURE9> m_vecTexture;
};

