#pragma once
#include "CGameObject.h"
#include "UIPanel.h"
namespace Engine {
	class CTransform;
	class CUiQuad;
}

class CLogoBack :
    public CGameObject
{

private:
	explicit CLogoBack(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoBack();
public:
	static CLogoBack* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};

private://component Caching
	CTransform* m_pTransform;
	CUiQuad* m_pQuad;
	UIPanel m_tPanel;

public:
	virtual void Free();
};

