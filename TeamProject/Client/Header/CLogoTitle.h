#pragma once
#include "CGameObject.h"
#include "UIPanel.h"
namespace Engine {
	class CTransform;
	class CUiQuad;
}
class CLogoTitle :
    public CGameObject
{
private:
	explicit CLogoTitle(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual ~CLogoTitle();
public:
	static CLogoTitle* Create(LPDIRECT3DDEVICE9 pGraphicDev);
	virtual			HRESULT		Ready_GameObject();
	virtual			_int		Update_GameObject(const _float& fTimeDelta);
	virtual			void		LateUpdate_GameObject(const _float& fTimeDelta);
	virtual			void		Render_GameObject() {};

private://component Caching
	CTransform* m_pTransform;
	CUiQuad* m_pQuad;
	UIPanel m_tPanel;
	wstring m_sTitle;
	ID3DXFont* m_pFont = nullptr;
	LPDIRECT3DTEXTURE9 m_pHintTex = nullptr;
	LPDIRECT3DSURFACE9 m_pNewSurf = nullptr;
	LPDIRECT3DSURFACE9 m_pOldSurf = nullptr;
public:
	virtual void Free();
};

