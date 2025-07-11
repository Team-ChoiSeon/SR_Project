#pragma once
#include "CBlock.h"
class CStairBlock :
    public CBlock
{
private:
    explicit CStairBlock(LPDIRECT3DDEVICE9 pDevice);
    virtual ~CStairBlock();

public:
    static CStairBlock* Create(LPDIRECT3DDEVICE9 pDevice);
    virtual HRESULT		Ready_GameObject();
    virtual _int		Update_GameObject(const _float& fTimeDelta);
    virtual void		LateUpdate_GameObject(const _float& fTimeDelta);
public:
    void Set_Distance(float distance) { m_fDistance = distance; };
private:
    _bool m_bFindTarget;
    float m_fDistance;
    float m_fnowHeight;
    CGameObject* m_pTarget;
private:
    void Free();
};

