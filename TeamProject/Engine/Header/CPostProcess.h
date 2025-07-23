#pragma once
#include "CBase.h"
BEGIN(Engine)
class ENGINE_DLL CPostProcess :
    public CBase
{
private:
    explicit CPostProcess(LPDIRECT3DDEVICE9 pDevice);
    virtual ~CPostProcess();
public:
    static CPostProcess* Create(LPDIRECT3DDEVICE9 pDevice);
    HRESULT Ready_Process();
    void BeginScene();  // 렌더 타겟 설정
    void EndScene();    // 후처리하고 백버퍼에 출력

public:
    void Start_Glitch(_float duration);
    void Start_Dead(_float duration);

private:
    _float m_fTotal = 2.f;
    _float m_fDuration = 2.f;
    _bool m_bEffect = false;

    LPDIRECT3DDEVICE9 m_pDevice = nullptr;

    LPDIRECT3DTEXTURE9 m_pRenderTexture = nullptr;
    LPDIRECT3DSURFACE9 m_pRenderSurf = nullptr;

    LPDIRECT3DSURFACE9 m_pBackBuffer= nullptr;

    LPDIRECT3DVERTEXBUFFER9 m_pVB;

    LPD3DXEFFECT m_pEffect = nullptr;
    int m_iEffectType = 0;
private:
    void Free() override;
};

END