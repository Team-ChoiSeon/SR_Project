#pragma once
#include "CBase.h"
BEGIN(Engine)
class ENGINE_DLL CShaderMgr :
    public CBase
{
    DECLARE_SINGLETON(CShaderMgr)
private:
    explicit CShaderMgr();
    virtual ~CShaderMgr();

public:
    HRESULT Ready_Shader(LPDIRECT3DDEVICE9 pDevice);
    void Load_AllShaders(const wstring& folderPath);
    LPD3DXEFFECT GetShader(const wstring& path);
    HRESULT ReloadShader(const wstring& path); // ÇÖ·Îµå
private:
    wstring ToWString(const string& str);
    string ToString(const wstring& wstr);
private:
    LPDIRECT3DDEVICE9 m_pDevice;
    unordered_map<wstring, LPD3DXEFFECT> m_mapShader;
private:
    void Free() override;
};
END
