#pragma once
#include "CResource.h"
class CTexture : public CResource
{
protected:
    explicit CTexture();

public:
    virtual ~CTexture();

public:
    static CTexture* Create();
    LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex = 0) const { return m_pTexture; }
    HRESULT Ready_Texture();
    HRESULT Load(LPDIRECT3DDEVICE9 pDevice, const wstring& filePath); // 경로 기반 로드
    void Bind(LPDIRECT3DDEVICE9 pDevice, _uint stage = 0);

    virtual void Free() override;


private:
    LPDIRECT3DBASETEXTURE9 m_pTexture = nullptr;
};

