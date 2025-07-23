#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine {
    class CUiQuad;
}

class CHealthBar :
    public Engine::CGameObject
{
private:
    explicit CHealthBar(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CHealthBar();
public:
    static CHealthBar* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			HRESULT		Ready_GameObject();
    virtual			_int		Update_GameObject(const _float& fTimeDelta);
    virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
    void Set_Pivot(_vec2 vCriteria);
    UIPanel& Get_Panel() { return m_tPanel; };
    void Set_Ratio(_float ratio);
private:
    CTransform* m_pTransform = nullptr;
    CUiQuad* m_pQuad = nullptr;
    UIPanel m_tPanel;

private:
    void Free();
};
