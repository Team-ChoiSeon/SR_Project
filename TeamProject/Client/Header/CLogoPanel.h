#pragma once
#include "CGameObject.h"
#include "UIPanel.h"

namespace Engine{
	class CTransform;
	class CUiQuad;
}

class CLogoPanel :
    public Engine::CGameObject
{
private:
	explicit CLogoPanel(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoPanel();
public:
	static CLogoPanel* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};

private:
private://component Caching
	CTransform* m_pTransform;
	CUiQuad* m_pQuad;
	UIPanel m_tPanel;

	unordered_map<wstring, CGameObject*> m_umUI;
public:
	virtual void Free();
};

