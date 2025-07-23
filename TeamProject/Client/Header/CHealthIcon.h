#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine {
    class CUiQuad;
}

class CHealthIcon :
    public Engine::CGameObject
{
private:
    explicit CHealthIcon(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual ~CHealthIcon();
public:
    static CHealthIcon* Create(LPDIRECT3DDEVICE9 pGraphicDev);
    virtual			HRESULT		Ready_GameObject();
    virtual			_int		Update_GameObject(const _float& fTimeDelta);
    virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);

public:
    void Set_Pivot(_vec2 vCriteria);

private:
    CTransform* m_pTransform = nullptr;
    CUiQuad* m_pQuad = nullptr;
    UIPanel m_tPanel;
private:
    void Free();
};