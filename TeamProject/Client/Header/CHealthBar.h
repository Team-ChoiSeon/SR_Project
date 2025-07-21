#pragma once
#include "CGameObject.h"
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
    void Set_Pos(_vec3 vCriteria, _vec3 vPos);

private:
    CTransform* m_pTransform = nullptr;
    CUiQuad* m_pHealthBar = nullptr;
private:
    void Free();
};
