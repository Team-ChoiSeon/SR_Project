#pragma once
#include "CResource.h"
BEGIN(Engine)

class ENGINE_DLL CTexture : public CResource
{
protected:
    explicit CTexture();

public:
    virtual ~CTexture();

public:
    static CTexture* Create();

public:
    const wstring& Get_Key() const { return m_wKey; }
    void Set_Key(const wstring& key) {  m_wKey =key; }

    LPDIRECT3DBASETEXTURE9 Get_Texture(_uint iIndex = 0) const { return m_pTexture; }

    HRESULT Ready_Texture();
    HRESULT Load(LPDIRECT3DDEVICE9 pDevice, const wstring& filePath); // 경로 기반 로드
    void Bind(LPDIRECT3DDEVICE9 pDevice, _uint stage = 0);

    virtual void Free() override;


private:
    LPDIRECT3DBASETEXTURE9 m_pTexture = nullptr;
    wstring m_wKey;
};

END